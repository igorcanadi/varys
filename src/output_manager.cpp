#include "output_manager.h"

OutputManager::OutputManager(boost::property_tree::ptree &config,
        boost::shared_ptr<Output> _output,
        boost::shared_ptr<Queue<ptrRecord> > _outputBuffer) {
    this->outputBuffer_ = _outputBuffer;
    this->output_ = _output;

    try {
        this->flushFrequency_ = config.get <int> ("flush_frequency");
    } catch (boost::property_tree::ptree_bad_path e) {
        std::cerr << "error reading config for output managers\n";
        exit(1);
    }
    this->exitInitiated_ = false;
}

OutputManager::~OutputManager() {
    this->cleanExit();
}

void OutputManager::run() {
    pthread_create(&this->thread_, NULL,
            &OutputManager::callOutputThread, (void *) this);
}

void OutputManager::cleanExit() {
    printf("OutputManager: clean exit called\n");

    if (this->exitInitiated_)
        return;

    this->exitInitiated_ = true;
    pthread_join(this->thread_, NULL);
}

void * OutputManager::callOutputThread(void *arg) {
    ((OutputManager *)arg)->outputThread();
}

void OutputManager::outputThread() {
    int nextFlush = time(NULL) + this->flushFrequency_;

    while (!this->exitInitiated_) {
        int now = time(NULL);

        if (nextFlush <= now) {
            // flush now
            std::vector <ptrRecord> records;
            this->outputBuffer_->popMany(records, -1, /* block */ false);
            printf("Flushing %d records\n", records.size());
            if (records.size()) {
                this->output_->outputRecords(records);
            }
            nextFlush = now + this->flushFrequency_;
        }

        int needToSleep = nextFlush - now;
        // I do this just to reduce exit latency.
        // If this thread sleeps, we can't exit. We don't want that
        // TODO make this configurable?
        if (needToSleep > 5) needToSleep = 5;
        if (needToSleep > 0)
            sleep(needToSleep);
    }
}
