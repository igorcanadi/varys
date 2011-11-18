#include <cstdio>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <glog/logging.h>
#include "queue.h"
#include "record.h"
#include "mysql_output.h"
#include "sensor.h"
#include "sensor_factory.h"
#include "sensor_manager.h"
#include "output_manager.h"

void test_mysql(boost::property_tree::ptree &config) {
    LOG(INFO) << "Running MySQLOutput test";
    MySQLOutput *db = new MySQLOutput(config);

    int ret = db->outputRecords(std::vector<ptrRecord> (1, ptrRecord(new Record(10, time(NULL), 1.5))));
    CHECK(ret == 0);

    LOG(INFO) << "MySQLOutput test OK";
    delete db;
}

void test_snmp_sensor(boost::property_tree::ptree &config) {
    LOG(INFO) << "Running SNMPSensor test";
    std::vector <boost::shared_ptr<Sensor> > sensors;

    try {
        SensorFactory::createSensors(sensors, config.get_child("sensors"));
    } catch (boost::property_tree::ptree_bad_path e) {
        LOG(FATAL) << "Error reading configuration for sensors";
    }

    LOG(INFO) << "Number of sensors created: " << sensors.size();

    ptrRecord record(new Record());
    for (int i = 0; i < sensors.size(); ++i) {
        LOG(INFO) << "Querying sensor " << sensors[i]->getSensorID();
        CHECK(sensors[i]->getRecord(record) == 0);
        LOG(INFO) << "Got: " << record->getSensorID() << " "
           << record->getTimestamp() << " "
           << record->getValue();
    }
    LOG(INFO) << "SNMPSensor test OK";
}

void test_queue() {
    LOG(INFO) << "Running queue test";
    Queue <int> ob;

    ob.push(2);
    ob.push(3);

    CHECK(ob.pop() == 2);
    std::vector <int> t(3, 8);

    ob.pushMany(t);

    ob.popMany(t, -1);
    CHECK(t.size() == 9);
    for (int i = 0; i < t.size(); ++i) {
        CHECK(t[i] == 3);
    }
    CHECK(ob.empty() == true);
    LOG(INFO) << "queue test OK";
}

void test_sensor_manager(boost::property_tree::ptree &config) {
    LOG(INFO) << "Testing SensorManager";

    boost::shared_ptr<Queue<ptrRecord> > outputBuffer(new Queue<ptrRecord>);
    SensorManager sm(config, outputBuffer);
    sm.run();
    sleep(10);
    sm.cleanExit();

    CHECK(outputBuffer->empty() == false);
    LOG(INFO) << "Got these records:";
    while (!outputBuffer->empty()) {
        ptrRecord record = outputBuffer->pop();
        LOG(INFO) << record->getSensorID() << record->getTimestamp() << record->getValue();
    }
    LOG(INFO) << "SensorManager test OK";
}

void test_output_manager(boost::property_tree::ptree &config) {
    LOG(INFO) << "Testing OutputManager";

    boost::shared_ptr<Queue<ptrRecord> > outputBuffer(new Queue<ptrRecord>);
    boost::shared_ptr<Output> output =
        boost::shared_ptr<Output> (new MySQLOutput(config));
    OutputManager *om = new OutputManager(config, output, outputBuffer);

    om->run();

    sleep(1);
    outputBuffer->push(ptrRecord(new Record(-1, time(NULL), 5.0)));
    outputBuffer->push(ptrRecord(new Record(-1, time(NULL), 6.0)));
    sleep(7);
    // they have to be flushed now
    CHECK(outputBuffer->empty());
    outputBuffer->push(ptrRecord(new Record(-1, time(NULL), 5.0)));
    outputBuffer->push(ptrRecord(new Record(-1, time(NULL), 6.0)));
    sleep(11);
    CHECK(outputBuffer->empty());

    om->cleanExit();

    delete om;
    LOG(INFO) << "OutputManager test OK";
}

void test_all_together(boost::property_tree::ptree &config) {
    LOG(INFO) << "Running complete system test";

    boost::shared_ptr<Queue<ptrRecord> > outputBuffer(new Queue<ptrRecord>);
    boost::shared_ptr<Output> output =
        boost::shared_ptr<Output> (new MySQLOutput(config));
    OutputManager *om = new OutputManager(config, output, outputBuffer);
    SensorManager *sm = new SensorManager(config, outputBuffer);

    sm->run();
    om->run();
    // run for 1hour
    sleep(60*60);

    delete om;
    delete sm;

    LOG(INFO) << "Complete system test DONE";
}
