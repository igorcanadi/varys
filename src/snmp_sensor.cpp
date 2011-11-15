#include "snmp_sensor.h"

int SNMPSensor::getRecord(Record &record) {
    int retval = -1;

    if (!this->libraryInitialized_) {
        init_snmp("snmpsensor");
        this->libraryInitialized_ = true;
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
        std::cerr << "Can't open connection to " << this->host_ << std::endl;
        return -1;
    }

    // define object id we'll query
    oid objectID[MAX_OID_LEN];
    size_t objectIDLen = MAX_OID_LEN;
    snmp_parse_oid(".1.3.6.1.2.1.1.1.0", objectID, &objectIDLen);

    // define request
    netsnmp_pdu *req;
    req = snmp_pdu_create(SNMP_MSG_GET);
    snmp_add_null_var(req, objectID, objectIDLen);

    // send query
    netsnmp_pdu *response;
    int status = snmp_synch_response(session, req, &response);

    printf("response\n");

    if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
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
