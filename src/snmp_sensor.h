#ifndef _SNMP_SENSOR_H
#define _SNMP_SENSOR_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "record.h"
#include "sensor.h"

#define INVALID_VALUE -1e10

class SNMPSensor : public Sensor {
public:
    virtual int getRecord(Record &record);

    SNMPSensor(int _sensorID, std::string _host,
            std::string _community, std::string _oid) :
        Sensor(_sensorID),
        host_(_host),
        community_(_community),
        oid_(_oid) {}

private:
    std::string host_, community_, oid_;
    bool libraryInitialized_;
};

#endif
