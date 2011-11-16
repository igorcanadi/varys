#include "mysql_output.h"

MySQLOutput::MySQLOutput(const boost::property_tree::ptree &config) {
    try {
        this->host_ = config.get<std::string>("mysql.host");
        this->user_ = config.get<std::string>("mysql.user");
        this->password_ = config.get<std::string>("mysql.password");
        this->db_ = config.get<std::string>("mysql.db");
        this->table_ = config.get<std::string>("mysql.table");
    } catch (boost::property_tree::ptree_bad_path e) {
        std::cerr << "error reading config for mysql\n";
    }
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
