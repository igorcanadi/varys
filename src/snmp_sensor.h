#ifndef _SNMP_SENSOR_H
#define _SNMP_SENSOR_H

#include <string>

#include "record.h"
#include "sensor.h"

class SNMPSensor : public Sensor{
public:
    /**
     * @returns file descriptor to wait for IO
     */
    int sendQueryAsync();
    /**
     * If IO is ready, it's non-blocking.
     * It can also be called with IO not ready,
     * but then it's blocking
     * @returns 0 on success, -1 on error
     */
    int getRecordAsync(Record &record);

    // TODO read host from config file
    SNMPSensor(int _sensorID, std::string _host) :
        Sensor(_sensorID),
        host_(_host) {}

private:
    std::string host_;
    int socket;
};

#endif
