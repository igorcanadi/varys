#include "sensor.h"

void Sensor::createRecord(Record &record, double value) {
    record.setSensorID(this->sensorID_);
    record.setTimestamp(time(NULL));
    record.setValue(value);
}
