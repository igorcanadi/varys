#include "snmp_sensor.h"

bool SNMPSensor::libraryInitialized_ = false;
boost::mutex SNMPSensor::mutex_;

int SNMPSensor::getRecord(ptrRecord record) {
    int retval = -1;

    // SNMP library might not be thread-safe
    boost::lock_guard<boost::mutex> lock(SNMPSensor::mutex_);

    if (!SNMPSensor::libraryInitialized_) {
        init_snmp("snmpsensor");
        SNMPSensor::libraryInitialized_ = true;
    }

    // initialize session
    netsnmp_session sess;
    snmp_sess_init(&sess);
    sess.version = SNMP_VERSION_1;
    sess.peername = strdup(this->host_.c_str());
    sess.community = (u_char *)strdup(this->community_.c_str());
    sess.community_len = this->community_.size();

    // open connection
    netsnmp_session *session;
    session = snmp_open(&sess);
    if (!session) {
        LOG(ERROR) << "Can't open connection to " << this->host_;
        return -1;
    }

    // define object id we'll query
    oid objectID[MAX_OID_LEN];
    size_t objectIDLen = MAX_OID_LEN;
    snmp_parse_oid(this->oid_.c_str(), objectID, &objectIDLen);

    // define request
    netsnmp_pdu *req;
    req = snmp_pdu_create(SNMP_MSG_GET);
    snmp_add_null_var(req, objectID, objectIDLen);

    // send query
    netsnmp_pdu *response;
    int status = snmp_synch_response(session, req, &response);

    if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
        // TODO Create record here from the actual variable read from the server
        this->createRecord(record, rand());

        for (netsnmp_variable_list *vars = response->variables; vars; vars = vars->next_variable) {
            print_variable(vars->name, vars->name_length, vars);
        }
        retval = 0;
    }

    if (response)
        snmp_free_pdu(response);
    snmp_close(session);

    return retval;
}
