/*
 * DBCassandra.cpp
 *
 *  Created on: Feb 24, 2016
 *      Author: michelo
 */

#include "DBCassandra.h"
#include <common/debug/core/debug.h>
namespace common {
namespace misc {
namespace data {

DBCassandra::DBCassandra() {
	// TODO Auto-generated constructor stub
	cluster = cass_cluster_new();
}

DBCassandra::~DBCassandra() {
	// TODO Auto-generated destructor stub
}

void DBCassandra::print_error(CassFuture* future) {
  const char* message;
  size_t message_length;
  cass_future_error_message(future, &message, &message_length);
  fprintf(stderr, "Error: %.*s\n", (int)message_length, message);
}
int DBCassandra::execute_query(const std::string& query){
	CassError rc = CASS_OK;
	CassStatement* statement = cass_statement_new(query.c_str(), 0);
	CassFuture* future = cass_session_execute(session, statement);

	cass_future_wait(future);

	  rc = cass_future_error_code(future);
	  if (rc != CASS_OK) {
	    print_error(future);
	  }

	  cass_future_free(future);
	  cass_statement_free(statement);
	  if(rc==CASS_OK)
		  return 0;
	  return rc;
}
int DBCassandra::connect(){
	std::string lista;
	if(servers.empty()){
		DERR("no servers are specified");
		return -1;
	}
	if(cluster==NULL){
		DERR("no cluster created");
		return -1;
	}
	for(std::vector <std::string>::iterator i=servers.begin();i!=servers.end();i++){

		lista+=*i;
		if((i+1)!=servers.end()){
			lista+=",";
		}
	}
	 cass_cluster_set_contact_points(cluster, lista.c_str());
	 CassError rc = CASS_OK;
	 CassFuture* future = cass_session_connect(session, cluster);

	  cass_future_wait(future);
	  rc = cass_future_error_code(future);
	  if (rc != CASS_OK) {
	    print_error(future);
	  }
	  cass_future_free(future);

	  return rc==CASS_OK?0:rc;

}

int DBCassandra::createDataSet(DataSet*dset){
	CassError rc = CASS_OK;
	CassFuture* future = NULL;
	std::stringstream query;

	query<<"INSERT INTO "<<name<<"(uuid,event_time,"
	  //const char* query = "SELECT * FROM examples.basic WHERE key = ?";
	 const char* query = "INSERT INTO examples.push (uuid, event_time, data) VALUES (?, ?, ?);";

	  future = cass_session_prepare(session, query);
	  cass_future_wait(future);

	  rc = cass_future_error_code(future);
	  if (rc != CASS_OK) {
	    print_error(future);
	  } else {
	    *prepared = cass_future_get_prepared(future);
	  }

	  cass_future_free(future);

	  return rc;
}


} /* namespace data */
} /* namespace misc */
} /* namespace common */
