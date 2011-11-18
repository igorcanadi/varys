#include "sensor.h"

int Sensor::getSensorID() const {
    return this->sensorID_;
}

void Sensor::createRecord(ptrRecord record, double value) {
    record->setSensorID(this->sensorID_);
    record->setTimestamp(time(NULL));
    record->setValue(value);
}
