#include <cstdio>
#include "record.h"
#include "mysql_output.h"

int main() {
    MySQLOutput *db = new MySQLOutput;

    int ret = db->outputRecords(std::vector<Record> (1, Record(10, time(NULL), 1.5)));
    printf("ret: %d\n", ret);

    delete db;
    return 0;
}
