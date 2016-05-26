/*
 * DBCassandra.h
 *
 *  Created on: Feb 24, 2016
 *      Author: michelo
 */

#ifndef DATA_CASSANDRA_DBCASSANDRA_H_
#define DATA_CASSANDRA_DBCASSANDRA_H_
#include <common/misc/data/DBbase.h>
#include <vector>
#include "cassandra.h"

namespace common {
namespace misc {
namespace data {

class DBCassandra: public DBbase<DBCassandra>{

	friend class DBbase<DBCassandra>;
	CassCluster* cluster;
	CassSession* session;

	typedef std::map<std::string,const CassPrepared*> prepared_t;

	prepared_t prepared;
	void print_error(CassFuture* future);
	const char* dataTypeToCassandra(dataTypes t);
	CassError execStatement(CassStatement* statement,bool wait=true);
	CassError waitAll();
	bool is_connected;
	int n_threads;
	std::string replication_factor;
	CassFuture**future_answer;
	int curr_answer;
	 uint64_t counter;

private:

	//;

	DBCassandra(const std::string name);
	~DBCassandra();

public:


	int connect();
		/**
			 * disconnects from database
			 * @return 0 on success
			 * */
	int disconnect();

	int executeQuery(const std::string& str);

	int pushData(const DataSet&ds,uint64_t ts=0);
	int pushData(const std::string &tbl,const std::string &key,std::string& ds,uint64_t ts=0);

	int queryData(const DataSet& ds,datasetRecord_t& set ,int64_t startTime=0,int64_t endTime=-1);
	int queryData(const std::string& tblname,const std::string& key,blobRecord_t& set,int64_t startTime=0,int64_t endTime=-1);
	int dropData(const DataSet&);
};

} /* namespace data */
} /* namespace misc */
} /* namespace common */

#endif /* DATA_CASSANDRA_DBCASSANDRA_H_ */
