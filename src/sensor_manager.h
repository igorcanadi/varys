#ifndef _SENSOR_MANAGER_H
#define _SENSOR_MANAGER_H

#include <ctime>
#include <cstdlib>
#include <set>
#include <utility>
#include <pthread.h>
#include <boost/property_tree/ptree.hpp>
#include <glog/logging.h>

#include "queue.h"
#include "record.h"
#include "sensor.h"
#include "sensor_factory.h"

class SensorManager {
public:
    // initialize the threads and run them
    void run();
    // kill the threads and wait for them to stop
    void cleanExit();
    // query sensors from time to time
    void queryThread();
    // schedule the queries thread (only 1, always index 0)
    void scheduleThread();
    // static methods used as callbacks for creating threads
    static void * callScheduleThread(void *arg);
    static void * callQueryThread(void *arg);

    SensorManager(boost::property_tree::ptree &config,
            boost::shared_ptr<Queue<ptrRecord> > _outputBuffer);
    ~SensorManager();
private:
    // how many threads to create
    int maxNumberOfThreads_;
    // number of retries
    int numberOfRetries_;
    // vector of all sensors
    std::vector <boost::shared_ptr<Sensor> > sensors_;
    // we will use this to make an event loop
    std::set <std::pair <int, int> > timeouts_;
    // where should we put the records once we get them
    boost::shared_ptr<Queue<ptrRecord> > outputBuffer_;
    // threads will read their jobs from here
    Queue <int> jobQueue_;
    // thread pool. 0th thread is always the scheduler one
    std::vector <pthread_t> threads_;
    // if this is true, all threads must kill themselves
    bool exitInitiated_;
};

#endif
