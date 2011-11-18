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

void test_mysql(boost::property_tree::ptree &config) {
    MySQLOutput *db = new MySQLOutput(config);

    int ret = db->outputRecords(std::vector<Record> (1, Record(10, time(NULL), 1.5)));
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

    Record record;
    for (int i = 0; i < sensors.size(); ++i) {
        printf("ret: %d\n", sensors[i]->getRecord(record));
        printf("%d %d %lf\n", record.getSensorID(), record.getTimestamp(), record.getValue());
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
