#include <cstdio>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include "tests.h"
#include "sensor_manager.h"

int main(int argc, char *argv[]) {
    boost::property_tree::ptree config;
    try {
        if (argc < 2) {
            read_info("varys.conf", config);
        } else {
            read_info(argv[1], config);
        }
    } catch (boost::property_tree::info_parser_error e) {
        printf("error\n");
    }

    test_sensor_manager(config);
    return 0;
    test_mysql(config);
    test_snmp_sensor(config);
    test_queue();

    return 0;
}
