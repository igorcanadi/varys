#include "sensor_manager.h"

SensorManager::SensorManager(boost::property_tree::ptree &config,
        boost::shared_ptr<Queue<ptrRecord> > _outputBuffer) {
    this->outputBuffer_ = _outputBuffer;
    try {
        SensorFactory::createSensors(this->sensors_, config.get_child("sensors"));
        this->maxNumberOfThreads_ = config.get <int> ("max_num_of_sensor_threads");
    } catch (boost::property_tree::ptree_bad_path e) {
        std::cerr << "error reading config for sensors\n";
        exit(1);
    }

    this->exitInitiated_ = false;
}

SensorManager::~SensorManager() {
    this->cleanExit();
}

void SensorManager::run() {
    if (maxNumberOfThreads_ < 2) {
        std::cerr << "I need to have at least 2 threads\n";
        exit(1);
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
    printf("SensorManager: clean exit called\n");

    if (this->exitInitiated_)
        return;

    this->exitInitiated_ = true;

    for (int i = 0; i < this->threads_.size(); ++i) {
        pthread_join(this->threads_[i], NULL);
    }
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
            printf("scheduling sensor %d\n", nextJob.second);
            this->jobQueue_.push(nextJob.second);
            int nextOcurrence = now;
            nextOcurrence += this->sensors_[nextJob.second]->getQueryFrequency();
            this->timeouts_.insert(std::make_pair(nextOcurrence, nextJob.second));
            this->timeouts_.erase(this->timeouts_.begin());
        }

        int needToSleep = this->timeouts_.begin()->first - now;
        // I do this just to reduce exit latency. If this thread sleeps, we can't
        // exit
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
        printf("dobio poso %d\n", nextJob);
        ptrRecord record(new Record());
        this->sensors_[nextJob]->getRecord(record);
        // TODO check if record is valid
        this->outputBuffer_->push(record);
        nextJob = this->jobQueue_.pop();
    }
}
