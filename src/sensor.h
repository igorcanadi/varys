#ifndef _SENSOR_H
#define _SENSOR_H

#include "record.h"
#include <ctime>

class Sensor {
public:
    /**
     * @returns 0 on success, -1 on error
     */
    virtual int getRecordSync(Record &record);
    /**
     * @returns file descriptor to wait for IO
     */
    virtual int sendQueryAsync() = 0;
    /**
     * @returns 0 on success, -1 on error
     */
    virtual int getRecordAsync(Record &record) = 0;

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
