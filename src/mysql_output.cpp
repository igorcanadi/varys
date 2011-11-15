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
    // mysql library throws exceptions
    try {
        if (!this->connection_.connected())
            this->connect();

        mysqlpp::Query query = this->connection_.query();
        query << "INSERT INTO " << this->table_
            << " (sensor_id, timestamp, value) "
            << " VALUES ";

        bool addComa = false;
        // TODO insert by chunks
        for (int i = 0; i < records.size(); ++i) {
            if (addComa) query << ",";
            query << "(" << records[i].getSensorID() << ","
                << records[i].getTimestamp() << ","
                << records[i].getValue() << ")";
            addComa = true;
        }

        mysqlpp::SimpleResult res = query.execute();

        // if we're here, insertion succeeded
        std::cerr << "Inserted " << records.size() <<
            " records into " << this->table_ << std::endl;

    } catch (const std::exception& er) {
        std::cerr << "Error: " << er.what() << std::endl;
        return -1;
    }


    return 0;
}

void MySQLOutput::connect() {
    this->connection_.connect(db_.c_str(), host_.c_str(),
            user_.c_str(), password_.c_str());
}
