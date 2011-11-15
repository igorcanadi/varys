#ifndef _MYSQL_OUTPUT_H
#define _MYSQL_OUTPUT_H

#include <iostream>
#include <vector>
#include <string>
#include <mysql++.h>
#include <exception>

#include "record.h"
#include "output.h"

class MySQLOutput : public Output {
public:
    /**
     * @returns 0 on success, -1 on error
     */
    int outputRecords(const std::vector <Record>& records);
    MySQLOutput();

private:
    /*
     * throws exceptions
     */
    void connect();

    std::string host_, user_, password_, db_, table_;
    mysqlpp::Connection connection_;
};

#endif
