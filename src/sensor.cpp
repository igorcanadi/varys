#include "sensor.h"

int Sensor::getRecordSync(Record &record) {
    if (!this->sendQueryAsync())
        return -1;

    if (!this->getRecordSync(record))
        return -1;

    return 1;
}

void Sensor::createRecord(Record &record, double value) {
    record.setSensorID(this->sensorID_);
    record.setTimestamp(time(NULL));
    record.setValue(value);
}
