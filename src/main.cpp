#include <cstdio>
#include "record.h"
#include "mysql_output.h"
#include "snmp_sensor.h"

void test_mysql() {
    MySQLOutput *db = new MySQLOutput;

    int ret = db->outputRecords(std::vector<Record> (1, Record(10, time(NULL), 1.5)));
    printf("ret: %d\n", ret);

    delete db;
}

void test_snmp_sensor() {
    SNMPSensor sensor(1, "test.net-snmp.org", "demopublic");

    Record record;
    printf("ret: %d\n", sensor.getRecord(record));
}

int main() {
    test_snmp_sensor();

    return 0;
}
