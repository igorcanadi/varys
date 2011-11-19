#ifndef _MYSQL_OUTPUT_H
#define _MYSQL_OUTPUT_H

#include <vector>
#include <string>
#include <mysql++/mysql++.h>
#include <exception>
#include <boost/property_tree/ptree.hpp>
#include <glog/logging.h>

#include "record.h"
#include "output.h"

class MySQLOutput : public Output {
public:
    /**
     * @returns 0 on success, -1 on error
     */
    int outputRecords(const std::vector <ptrRecord>& records);
    MySQLOutput(const boost::property_tree::ptree &config);

private:
    /*
     * throws exceptions
     */
    void connect();

    std::string host_, user_, password_, db_, table_;
    mysqlpp::Connection connection_;
};

#endif
