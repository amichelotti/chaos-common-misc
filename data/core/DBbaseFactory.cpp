/*
 * DBbase.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */

#include "DBbaseFactory.h"
#include <common/misc/data/cassandra/DBCassandra.h>
namespace common {
namespace misc {
namespace data {
boost::mutex DBbaseFactory::mutex;


std::map<std::string,DBbase*> DBbaseFactory::instances;

 DBbase* DBbaseFactory::getInstance(const std::string& type,const std::string& name )
{
			boost::mutex::scoped_lock l(mutex);
			typename std::map<std::string,  DBbase*>::iterator i=instances.find(name);
			if(i!=instances.end()){
				return (i->second);
			}
			DBbase*ret=NULL;
			if(type== "cassandra"){
				ret =new DBCassandra(name);
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



