/*
 * DBbase.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */

#include "DBbaseFactory.h"
#ifdef CASSANDRA_DRIVER
#include <common/misc/data/cassandra/DBCassandra.h>
#endif
namespace common {
namespace misc {
namespace data {
ChaosMutex DBbaseFactory::mutex;


std::map<std::string,DBbase*> DBbaseFactory::instances;

 DBbase* DBbaseFactory::getInstance(const std::string& type,const std::string& name )
{
			ChaosLockGuard l(mutex);
			typename std::map<std::string,  DBbase*>::iterator i=instances.find(name);
			if(i!=instances.end()){
				return (i->second);
			}
			DBbase*ret=NULL;
			if(type== "cassandra"){
#ifdef CASSANDRA_DRIVER
				ret =new DBCassandra(name);
#else
				
				ERR("cassandra support not enabled -DCASSANDRA_DRIVER=ON");
				return NULL;
#endif				
			} else {
				ERR("uknown database type:%s",type.c_str());
				return NULL;
			}

			instances[name]=ret;
			return ret;
		}
} /* namespace data */
} /* namespace misc */
} /* namespace common */



