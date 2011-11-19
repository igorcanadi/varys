#include "sensor_manager.h"

SensorManager::SensorManager(boost::property_tree::ptree &config,
        boost::shared_ptr<Queue<ptrRecord> > _outputBuffer) {
    this->outputBuffer_ = _outputBuffer;
    try {
        SensorFactory::createSensors(this->sensors_, config.get_child("sensors"));
        this->maxNumberOfThreads_ = config.get <int> ("max_num_of_sensor_threads");
        this->numberOfRetries_ = config.get <int> ("sensor_query_retries");
    } catch (boost::property_tree::ptree_bad_path e) {
        LOG(FATAL) << "Error reading configuration for sensor manager";
    }

    this->exitInitiated_ = false;
}

SensorManager::~SensorManager() {
    this->cleanExit();
}

void SensorManager::run() {
    if (maxNumberOfThreads_ < 2) {
        LOG(FATAL) << "I need to have at least 2 threads";
    }
    this->threads_.resize(maxNumberOfThreads_);

    for (int i = 0; i < sensors_.size(); ++i) {
        timeouts_.insert(std::make_pair(0, i));
    }

    pthread_create(&this->threads_[0], NULL,
            &SensorManager::callScheduleThread, (void *) this);
    for (int i = 1; i < this->threads_.size(); ++i) {
        pthread_create(&this->threads_[i], NULL,
                &SensorManager::callQueryThread, (void *) this);
    }
}

void SensorManager::cleanExit() {
    LOG(INFO) << "SensorManager: clean exit called";

    if (this->exitInitiated_)
        return;

    this->exitInitiated_ = true;

    for (int i = 0; i < this->threads_.size(); ++i) {
        pthread_join(this->threads_[i], NULL);
    }

    LOG(INFO) << "SensorManager: exited";
}

void * SensorManager::callScheduleThread(void *arg) {
    ((SensorManager *)arg)->scheduleThread();
}

void * SensorManager::callQueryThread(void *arg) {
    ((SensorManager *)arg)->queryThread();
}

void SensorManager::scheduleThread() {
    while (!this->exitInitiated_) {
        int now = time(NULL);

        while (this->timeouts_.begin()->first <= now) {
            std::pair <int, int> nextJob = *this->timeouts_.begin();
            LOG(INFO) << "Scheduling sensor query " << nextJob.second;
            this->jobQueue_.push(nextJob.second);
            int nextOcurrence = now;
            nextOcurrence += this->sensors_[nextJob.second]->getQueryFrequency();
            this->timeouts_.insert(std::make_pair(nextOcurrence, nextJob.second));
            this->timeouts_.erase(this->timeouts_.begin());
        }

        int needToSleep = this->timeouts_.begin()->first - now;
        // I do this just to reduce exit latency. If this thread sleeps, we can't
        // exit
        // TODO make this configurable?
        if (needToSleep > 5) needToSleep = 5;
        sleep(needToSleep);
    }

    // wake up all those threads waiting on job queue. 
    // they will check exitInitiated_ and kill themselves
    for (int i = 0; i < 2*this->maxNumberOfThreads_; ++i) {
        this->jobQueue_.push(-1);
    }

}

void SensorManager::queryThread() {
    int nextJob = this->jobQueue_.pop();

    while (!this->exitInitiated_) {
        LOG(INFO) << "queryThread will query sensor with ID " 
            << this->sensors_[nextJob]->getSensorID();
        ptrRecord record(new Record());
        bool recordGood = false;

        for (int retry = 0; retry < this->numberOfRetries_; ++retry) {
            LOG(INFO) << "Querying " 
                << this->sensors_[nextJob]->getSensorID()
                << " retry " << retry;

            if (this->sensors_[nextJob]->getRecord(record) == 0) {
                recordGood = true;
                break;
            }

            // wait before retry
            usleep(200000);
        }
        if (recordGood) {
            LOG(INFO) << "Got response: " << record->getSensorID()
                << record->getTimestamp() << record->getValue();

            this->outputBuffer_->push(record);
        } else {
            LOG(ERROR) << "PANIC! Something wrong with sensor "
                << this->sensors_[nextJob]->getSensorID();
        }
        nextJob = this->jobQueue_.pop();
    }
}
