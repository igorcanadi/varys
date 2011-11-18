#ifndef _RECORD_H
#define _RECORD_H

#include <boost/shared_ptr.hpp>

class Record {
public:
	int getSensorID() const { return sensorID_; }
	int getTimestamp() const { return timestamp_; }
	double getValue() const { return value_; }

    void setSensorID(int _sensorID) { sensorID_ = _sensorID; }
    void setTimestamp(int _timestamp) { timestamp_ = _timestamp; }
    void setValue(double _value) { value_ = _value; }

    Record(int _sensorID, int _timestamp, double _value) :
        sensorID_(_sensorID), timestamp_(_timestamp), value_(_value) {}
    Record() {}
private:
    int sensorID_, timestamp_;
    double value_;
};

typedef boost::shared_ptr<Record> ptrRecord;

#endif
