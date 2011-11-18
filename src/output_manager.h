#ifndef _OUTPUT_MANAGER_H
#define _OUTPUT_MANAGER_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <set>
#include <utility>
#include <pthread.h>
#include <boost/property_tree/ptree.hpp>

#include "queue.h"
#include "record.h"
#include "output.h"

class OutputManager {
public:
    // initialize the threads and run them
    void run();
    // kill the threads and wait for them to stop
    void cleanExit();
    // output buffer contents. sleep. repeat
    void outputThread();
    // static method used as callback for creating threads
    static void * callOutputThread(void *arg);

    OutputManager(boost::property_tree::ptree &config,
            boost::shared_ptr<Output> _output,
            boost::shared_ptr<Queue<ptrRecord> > _outputBuffer);
    ~OutputManager();
private:
    // the guy that does the actual outputing
    boost::shared_ptr<Output> output_;
    // from where should we read the records
    boost::shared_ptr<Queue<ptrRecord> > outputBuffer_;
    // thread that's running
    pthread_t thread_;
    // if this is true, all threads must kill themselves
    bool exitInitiated_;
    // how many seconds to wait between two buffer flushes
    int flushFrequency_;
};

#endif
