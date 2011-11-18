#include <cstdio>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include "queue.h"
#include "record.h"
#include "mysql_output.h"
#include "sensor.h"
#include "sensor_factory.h"
#include "sensor_manager.h"
#include "output_manager.h"

void test_mysql(boost::property_tree::ptree &config) {
    MySQLOutput *db = new MySQLOutput(config);

    int ret = db->outputRecords(std::vector<ptrRecord> (1, ptrRecord(new Record(10, time(NULL), 1.5))));
    printf("ret: %d\n", ret);

    delete db;
}

void test_snmp_sensor(boost::property_tree::ptree &config) {
    std::vector <boost::shared_ptr<Sensor> > sensors;

    try {
        SensorFactory::createSensors(sensors, config.get_child("sensors"));
    } catch (boost::property_tree::ptree_bad_path e) {
        std::cerr << "error reading config for sensors\n";
    }

    printf("number of sensors: %d\n", sensors.size());

    ptrRecord record(new Record());
    for (int i = 0; i < sensors.size(); ++i) {
        printf("ret: %d\n", sensors[i]->getRecord(record));
        printf("%d %d %lf\n", record->getSensorID(), record->getTimestamp(), record->getValue());
    }
}

void test_queue() {
    Queue <int> ob;

    ob.push(3);
    ob.push(2);

    printf("%d\n", ob.pop());
    std::vector <int> t(3, 8);

    ob.pushMany(t);

    ob.popMany(t, 8);
    for (int i = 0; i < t.size(); ++i) {
        printf("%d ", t[i]);
    }
    printf("\n");
}

void test_sensor_manager(boost::property_tree::ptree &config) {
    boost::shared_ptr<Queue<ptrRecord> > outputBuffer(new Queue<ptrRecord>);
    SensorManager sm(config, outputBuffer);
    sm.run();
    sleep(10);
    sm.cleanExit();

    while (!outputBuffer->empty()) {
        ptrRecord record = outputBuffer->pop();
        printf("%d %d %lf\n", record->getSensorID(), record->getTimestamp(), record->getValue());
    }
}

void test_output_manager(boost::property_tree::ptree &config) {
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
    printf("outputBuffer empty: %d\n", outputBuffer->empty());
    outputBuffer->push(ptrRecord(new Record(-1, time(NULL), 5.0)));
    outputBuffer->push(ptrRecord(new Record(-1, time(NULL), 6.0)));
    sleep(11);

    om->cleanExit();

    delete om;
}

void test_all_together(boost::property_tree::ptree &config) {
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
}
