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
#include <boost/thread/mutex.hpp>
namespace common {
namespace misc {
namespace data {
typedef struct {
	uint64_t timestamp;
	std::string key;
	std::string data;
} dbrecord_t;
typedef std::map<int64_t,DataSet> datasetRecord_t;
typedef std::pair<int64_t, std::string> kv_t;
typedef  std::pair< kv_t, std::string> dbmap_t;
typedef std::vector< dbrecord_t > blobRecord_t;

class DBbaseFactory;


class DBbase {

private:
	DBbase(const std::string _name):name(_name){}
	friend class DBbaseFactory;

protected:
	std::vector <std::string> servers;
	std::string name;
	std::map<std::string,std::string> kv_parameters;




	public:

	DBbase(){}

	virtual ~DBbase(){}


	/**
	 * Add a Server url
	 * @param url server address/url
	 * @return 0 on success
	 * */
	 int addDBServer(std::string url);

	virtual int addDBServer(std::vector<std::string> urls);
	/**
		 * Set the database name
		 * @param name database name
		 * @return 0 on success
	*/
	 int setDBName(std::string _name);
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
	/**
	 * push a dataset, into a table corresponding to the dataset name. The timestamp is the primary key
	 * @param ds dataset pointer
	 * @param ts the timestamp in ms (0 use the client timestamp)
	 * @return 0 on success
	 * */
	virtual int pushData(const DataSet&ds,uint64_t ts=0)=0;
	/**
		 * push raw data, into a table, with a specified key (key and ts are primary keys)
		 * @param tbl table where to push
		 * @param key primary key
		 * @param ds raw data string
		 * @param ts timestamp ms (o use the client timestamp)
		 * @return 0 on success
		 * */
	virtual int pushData(const std::string &tbl,const std::string &key,std::string& ds,uint64_t ts=0)=0;
	/**
	 * Execute a query on the DB (the string depend on the DB)
	 * @param str query string
	 * @return 0 on success
	 * */
	virtual int executeQuery(const std::string& str)=0;
	/**
	 * Query a dataset(s) name in the given range of time
	 * @param ds dataset name and prototype
	 * @param startTime start search
	 * @param endTime end search
	 * @param set output set
	 * @return the number of dataset retrieved, negative if error
	 * */
	virtual int queryData(const DataSet& ds,datasetRecord_t& set ,int64_t startTime=0,int64_t endTime=-1)=0;
	/**
		 * Query a dataset(s) in the given range of time and keys
		 * @param tblname table to address
		 * @param key to use, if key is empty return a list of keys
		 * @param startTime start search
		 * @param endTime end search
		 * @param set output set
		 * @return the number of sets retrieved, negative if error
		 * */
	virtual int queryData(const std::string& tblname,const std::string& key,blobRecord_t& set,int64_t startTime=0,int64_t endTime=-1)=0;




	/**
	 *Drop the full dataset 
	 */
	virtual int dropData(const DataSet&)=0;
};
	std::ostream& operator<<(std::ostream& out,blobRecord_t&);

} /* namespace data */
} /* namespace misc */
} /* namespace common */

#endif /* DATA_CORE_DBBASE_H_ */
