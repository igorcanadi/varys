#ifndef _SENSOR_FACTORY_H
#define _SENSOR_FACTORY_H

#include <glog/logging.h>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/shared_ptr.hpp>

#include "sensor.h"
#include "snmp_sensor.h"

class SensorFactory {
public:
    static boost::shared_ptr<Sensor> createSensor(int sensorID,
            const boost::property_tree::ptree &config) {

        std::string protocol = config.get<std::string>("protocol");

        if (protocol == "snmp") {
            std::string host, community, oid;
            host = config.get<std::string>("host");
            community = config.get<std::string>("community");
            oid = config.get<std::string>("oid");
            int queryFrequency = config.get<int>("query_frequency");
            return boost::shared_ptr<Sensor>(
                    new SNMPSensor(sensorID, host, community,
                        oid, queryFrequency));
        } else {
            LOG(FATAL) << "Unrecognized Sensor protocol";
        }
    }

    static void createSensors(std::vector <boost::shared_ptr<Sensor> > &sensors,
            boost::property_tree::ptree &config) {
        sensors.clear();

        BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, config) {
            int sensorID;
            sscanf(v.first.c_str(), "%d", &sensorID);
            sensors.push_back(boost::shared_ptr<Sensor>(
                    SensorFactory::createSensor(sensorID, v.second)));
        }
    }
};

#endif
