#ifndef _RECORD_H
#define _RECORD_H

class Record {
public:
	int getSensorID() const { return sensorID_; }
	int getTimestamp() const { return timestamp_; }
	double getValue() const { return value_; }

    Record(int _sensorID, int _timestamp, double _value) :
        sensorID_(_sensorID), timestamp_(_timestamp), value_(_value) {}

private:
    int sensorID_, timestamp_;
    double value_;
};

#endif
