/*
 * DBbase.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */

#include "DBbase.h"

namespace common {
namespace misc {
namespace data {

DBbase::DBbase() {
	// TODO Auto-generated constructor stub

}
DBbase::DBbase(const std::string &_name):name(_name){

}
DBbase::~DBbase() {
	// TODO Auto-generated destructor stub
}

int DBbase::addDBServer(std::vector<std::string> urls){
	for(std::vector<std::string>::iterator i=urls.begin();i!=urls.end();i++){
		addDBServer(*i);
	}
	return 0;
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

