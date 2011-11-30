#ifndef _SNMP_SENSOR_H
#define _SNMP_SENSOR_H

#include <cstring>
#include <cstdlib>
#include <string>
#include <glog/logging.h>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/shared_ptr.hpp>

#include "record.h"
#include "sensor.h"

#define INVALID_VALUE -1e10

class SNMPSensor : public Sensor {
public:
    virtual int getRecord(ptrRecord record);
    virtual int getQueryFrequency() {
        // TODO read this from config
        return 5;
    }

    SNMPSensor(int _sensorID, std::string _host,
            std::string _community, std::string _oid) :
        Sensor(_sensorID),
        host_(_host),
        community_(_community),
        oid_(_oid) {}

private:
    std::string host_, community_, oid_;
    static bool libraryInitialized_;
    static boost::mutex mutex_;
};

#endif
