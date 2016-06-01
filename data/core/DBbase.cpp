/*
 * DBbase.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */

#include "DBbase.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <json/json.h>

namespace common {
namespace misc {
namespace data {

char _data_::format=0xf;

std::ostream& chformat(std::ostream& in,char f){
	_data_::format =f;
	return in;
}

std::ostream& operator<<(std::ostream& out,blobRecord_t& ret){
	boost::posix_time::ptime const time_epoch(boost::gregorian::date(1970, 1, 1));

	 out<<"{\"keys\":[";

	 for(int cnt=0;cnt<ret.size();cnt++){
		 boost::posix_time::ptime now = time_epoch + boost::posix_time::millisec(ret[cnt].timestamp);
		 out<<"{";
		 if(_data_::format&0x1){
			 out<<"\"key\":\""<<ret[cnt].key<<"\"";
		 }

		 if(_data_::format&0x2){

			 if(_data_::format&0x1){
				 out<<",";
			 }
			 out<<"\"ts\":\""<<boost::posix_time::to_simple_string(now)<<"\"";
		 }

		 if(_data_::format&0x4){

			 if(_data_::format&0x3){
			 		out<<",";
			 }
		 	out<<"\"data\":\""<<ret[cnt].data<<"\"";
		 }
		 out<<"}";
		if((cnt+1)<ret.size()){
			  out<<",";
		}
	 }
  out<<"]}";
  return out;
}

int64_t convertToEpoch(const std::string& ts){
	if(ts == "begin"){
		return 0;
	} else if(ts == "end"){
		return -1;
	}
	boost::posix_time::ptime const time_epoch(boost::gregorian::date(1970, 1, 1));
	boost::posix_time::ptime pts=boost::posix_time::time_from_string(ts);
	return(pts-time_epoch).total_milliseconds();

}

int DBbase::queryData(std::string& json_out,const std::string& json_param){
    Json::Reader					json_reader;
    Json::Value						json_parameter;
    std::string skey;
    int64_t its=-1,ite=-1;
    int ret;
    if(!json_reader.parse(json_param,json_parameter)){
    	ERR("cannot parse json %s",json_param.c_str());
    	return -1;
    }
    const Json::Value& tbl = json_parameter["tbl"];
    const Json::Value& key = json_parameter["key"];
    const Json::Value& ts = json_parameter["ts"];
    const Json::Value& te = json_parameter["te"];
    const Json::Value& select = json_parameter["select"];
    if(tbl.isNull()){
    	ERR("\"tbl\" parameter is needed");
    	return -2;
    }
    if(!key.isNull()){
    	skey=key.asString();
    }
    if(!ts.isNull()){
    	its = convertToEpoch(ts.asString());
    }
    if(!te.isNull()){
        	ite = convertToEpoch(te.asString());
    }
    blobRecord_t set;
	ret=queryData(tbl.asString(),skey,set,its,ite);
	if(ret==0){
		std::stringstream ss;

		if(select.isNull()){
			ss<<set;
			json_out=ss.str();
			return 0;
		} else {
			 ss<<"{\"keys\":[";


		    for (Json::ValueConstIterator it = select.begin();
		         it != select.end();
		         ++it) {
		        if(!it->isObject()) {

		        	ERR("bad syntax in select option");
		            return -3;
		        }

		}
	}

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



