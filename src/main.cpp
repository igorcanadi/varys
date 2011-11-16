#include <cstdio>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
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
    }
}


int main() {
    boost::property_tree::ptree config;
    try {
        read_info("varys.conf", config);
    } catch (boost::property_tree::info_parser_error e) {
        printf("error\n");
    }

    test_mysql(config);
    test_snmp_sensor(config);

    return 0;
}
