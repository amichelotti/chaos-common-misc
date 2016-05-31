/*
 * DBbase.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */

#include "DBbase.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
namespace common {
namespace misc {
namespace data {


std::ostream& operator<<(std::ostream& out,blobRecord_t& ret){
	boost::posix_time::ptime const time_epoch(boost::gregorian::date(1970, 1, 1));

	 out<<"{\"keys\":[";

	 for(int cnt=0;cnt<ret.size();cnt++){
		 boost::posix_time::ptime now = time_epoch + boost::posix_time::millisec(ret[cnt].timestamp);

		 out<<"{\"key\":\""<<ret[cnt].key<<"\",\"ts\":\""<<boost::posix_time::to_simple_string(now)<<"\"}";
		if((cnt+1)<ret.size())
			  out<<",";
	 }
  out<<"]}";
  return out;
}



int DBbase::addDBServer(std::string url){
			std::vector<std::string>::iterator i;
			for(i=servers.begin();i!=servers.end();i++){
				if(*i == url){
					return -1;
				}

			}

			servers.push_back(url);
			return 0;
		}

int DBbase::addDBServer(std::vector<std::string> urls){
		for(std::vector<std::string>::iterator i=urls.begin();i!=urls.end();i++){
			addDBServer(*i);
		}
		return 0;
	}

	 int DBbase::setDBName(std::string _name){
		 name = _name;
		 return 0;

	 }
 int DBbase::setDBParameters(std::string key,std::string value){
		  kv_parameters[key]=value;
	  }



} /* namespace data */
} /* namespace misc */
} /* namespace common */



