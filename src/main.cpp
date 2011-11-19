#include <vector>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <glog/logging.h>
#include "tests.h"
#include "sensor_manager.h"


int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "Started varys";

    // on failure signal, dumps the trace
    google::InstallFailureSignalHandler();

    // read the config
    boost::property_tree::ptree config;
    try {
        if (argc < 2) {
            read_info("varys.conf", config);
        } else {
            read_info(argv[1], config);
        }
    } catch (boost::property_tree::info_parser_error e) {
        LOG(FATAL) << "Error reading configuration";
    }

    test_sensor_manager(config);
    test_snmp_sensor(config);
    test_mysql(config);
    test_output_manager(config);
    test_queue();
    test_all_together(config);

    return 0;
}
