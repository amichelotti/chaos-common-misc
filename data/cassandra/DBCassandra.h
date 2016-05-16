/*
 * DBCassandra.h
 *
 *  Created on: Feb 24, 2016
 *      Author: michelo
 */

#ifndef DATA_CASSANDRA_DBCASSANDRA_H_
#define DATA_CASSANDRA_DBCASSANDRA_H_
#include <common/misc/data/core/DBbase.h>
#include <vector>
#include "cassandra.h"

namespace common {
namespace misc {
namespace data {

class DBCassandra: public DBbase {
	CassCluster* cluster;
	CassSession* session;
	const CassPrepared* prepared;

	void print_error(CassFuture* future);
public:
	DBCassandra();
	virtual ~DBCassandra();

	int connect();
		/**
			 * disconnects from database
			 * @return 0 on success
			 * */
	int disconnect()=0;

	int execute_query(const std::string& str);
	int createDataSet(DataSet*);

	int insertDataSet(DataSet*);
	std::vector<DataSet*> queryDataSet(int64_t startTime,int64_t endTime);
};

} /* namespace data */
} /* namespace misc */
} /* namespace common */

#endif /* DATA_CASSANDRA_DBCASSANDRA_H_ */
