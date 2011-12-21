#include <vector>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <glog/logging.h>
#include <signal.h>
#include "tests.h"
#include "sensor_manager.h"

int exit_initiated = 0;

void sighandler(int signum) {
    if (signum == SIGTERM) {
        exit_initiated = 1;
    }
}

void main_loop(boost::property_tree::ptree &config) {
    // this is the real action
    boost::shared_ptr<Queue<ptrRecord> > outputBuffer(new Queue<ptrRecord>);
    boost::shared_ptr<Output> output =
        boost::shared_ptr<Output> (new MySQLOutput(config));
    OutputManager *om = new OutputManager(config, output, outputBuffer);
    SensorManager *sm = new SensorManager(config, outputBuffer);

    signal(SIGTERM, sighandler);

    sm->run();
    om->run();

    do {
        if (exit_initiated) {
            break;
        }
        sleep(1);
    } while (true);

    delete om;
    delete sm;

    LOG(INFO) << "Exiting Varys";
}

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

    //test_sensor_manager(config);
    //test_snmp_sensor(config);
    //test_mysql(config);
    //test_output_manager(config);
    //test_queue();
    //test_all_together(config);

    main_loop(config);

    return 0;
}
