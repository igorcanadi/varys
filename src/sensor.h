#ifndef _SENSOR_H
#define _SENSOR_H

#include <ctime>

#include "record.h"

class Sensor {
public:
    /**
     * @returns 0 on success, -1 on error
     */
    virtual int getRecord(Record &record) = 0;

    Sensor(int _sensorID) :
        sensorID_(_sensorID) {}
protected:
    /**
     * @returns 0 on success, -1 on error
     */
    void createRecord(Record &record, double value);

private:
    int sensorID_;
};

#endif
