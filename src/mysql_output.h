#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <vector>
#include <string>
#include "record.h"
#include "output.h"

class MySQLOutput /*: public Output */{
public:
    /**
     * @returns 0 on success, -1 on error
     */
    int outputRecords(const std::vector <Record>& records);
    MySQLOutput();

private:
    std::string host_, user_, password_, db_, table_;
};

#endif
