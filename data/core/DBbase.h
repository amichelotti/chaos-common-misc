/*
 * DBbase.h
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */

#ifndef DATA_CORE_DBBASE_H_
#define DATA_CORE_DBBASE_H_
#include <vector>
#include <string>
#include <map>
#include "DataSet.h"
namespace common {
namespace misc {
namespace data {

class DBbase {
	std::vector <std::string> servers;
	std::string name;
	std::map<std::string,std::string> kv_parameters;

public:
	DBbase();
	DBbase(const std::string &_name);

	virtual ~DBbase();

	/**
	 * Add a Server url
	 * @param url server address/url
	 * @return 0 on success
	 * */
	virtual int addDBServer(std::string url);
	/**
		 * Set the database name
		 * @param name database name
		 * @return 0 on success
		 * */
	virtual int setDBName(std::string name);
	/**
			 * Set the database parameters
			 * @param key parameter name
			 * @param value parameter value
			 * @return 0 on success
			 * */
	virtual int setDBParameters(std::string key,std::string value);

	/**
	 * connects to database
	 * @return 0 on success
	 * */
	virtual int connect()=0;
	/**
		 * disconnects from database
		 * @return 0 on success
		 * */
	virtual int disconnect()=0;
	virtual int createDataSet(DataSet*)=0;
	virtual int execute_query(const std::string str)=0;
	virtual int insertDataSet(DataSet*)=0;
	virtual std::vector<DataSet*> queryDataSet(int64_t startTime,int64_t endTime)=0;
};

} /* namespace data */
} /* namespace misc */
} /* namespace common */

#endif /* DATA_CORE_DBBASE_H_ */
