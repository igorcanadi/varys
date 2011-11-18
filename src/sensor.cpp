#include "sensor.h"

void Sensor::createRecord(ptrRecord record, double value) {
    record->setSensorID(this->sensorID_);
    record->setTimestamp(time(NULL));
    record->setValue(value);
}
