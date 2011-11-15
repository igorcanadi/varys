#ifndef _SNMP_SENSOR_H
#define _SNMP_SENSOR_H

#include <iostream>
#include <cstring>
#include <string>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "record.h"
#include "sensor.h"

#define INVALID_VALUE -1e10

class SNMPSensor : public Sensor {
public:
    virtual int getRecord(Record &record);

    // TODO read host from config file
    SNMPSensor(int _sensorID, std::string _host, std::string _community) :
        Sensor(_sensorID),
        host_(_host),
        community_(_community) {}

private:
    std::string host_, community_;
    bool libraryInitialized_;
};

#endif
