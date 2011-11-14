#include "mysql_output.h"

MySQLOutput::MySQLOutput() {
    // TODO get this from config file
    this->host_ = "localhost";
    this->user_ = "igor";
    this->password_ = "UqYfh7dM";
    this->db_ = "energy";
    this->table_ = "sensor_data";
}

int MySQLOutput::outputRecords(const ::std::vector <Record>& records) {

    return 0;
}
