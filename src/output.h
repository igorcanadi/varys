#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <vector>
#include "record.h"

class Output {
public:
    /**
     * @returns 0 on success, -1 on error
     */
    virtual int outputRecords(const std::vector <Record>& records) = 0;
};

#endif
