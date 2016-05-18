/*
  This is free and unencumbered software inserreleased into the public domain.

  Anyone is free to copy, modify, publish, use, compile, sell, or
  distribute this software, either in source code form or as a compiled
  binary, for any purpose, commercial or non-commercial, and by any
  means.

  In jurisdictions that recognize copyright laws, the author or authors
  of this software dedicate any and all copyright interest in the
  software to the public domain. We make this dedication for the benefit
  of the public at large and to the detriment of our heirs and
  successors. We intend this dedication to be an overt act of
  relinquishment in perpetuity of all present and future rights to this
  software under copyright law.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.

  For more information, please refer to <http://unlicense.org/>
*/

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "DBCassandra.h"
#include <boost/program_options.hpp>
#include <boost/chrono.hpp>
#include <boost/date_time.hpp>
#define NPUSHES 1000000

using namespace common::misc::data;
namespace po=boost::program_options;

int main(int argc,char**argv) {
	int npushes=NPUSHES;
	std::string dbname,dbtable,key;
	std::vector<std::string> servers;
	int enable_raw=0;
	po::options_description desc("options");

	desc.add_options()("help","help");
	desc.add_options()("server,s",po::value<std::vector<std::string> >(&servers),"contacting servers");
	desc.add_options()("pushes,p",po::value<int >(&npushes)->default_value(NPUSHES),"number of pushes");
	desc.add_options()("dbname,n",po::value<std::string >(&dbname)->default_value("chaos"),"db name");
	desc.add_options()("dbtable,t",po::value<std::string >(&dbtable),"db table enable raw access mode");
	desc.add_options()("key",po::value<std::string >(&key)->default_value("testCU"),"key in case of (raw access)");
	po::variables_map vm;
	po::store(po::parse_command_line(argc,argv, desc),vm);
	po::notify(vm);
	if(vm.count("help")){
		desc.print(std::cout);
		return 0;
	}
	if(vm.count("server")==0){
		std::cerr<<"you must define a target DB server"<<std::endl;
		return -1;
	}
	if(vm.count("dbtable")){
			enable_raw=1;
	}
	DPRINT("connecting to %s",servers[0].c_str());
	DBCassandra cassandra(dbname);
	cassandra.addDBServer(servers);
	if(cassandra.connect()!=0){
		ERR("error connecting");
		return -1;
	}
	if(enable_raw){

		boost::posix_time::ptime start=boost::posix_time::microsec_clock::local_time();
		for(int cnt=0;cnt<npushes;cnt++){
			std::stringstream ss;
			ss<<"{\"prova\":"<<cnt<<"}";
			std::string ds=ss.str();
			if(cassandra.pushData(dbtable,key,ds,0)!=0){
			  std::cerr<<"Error pushing"<<std::endl;
			  return -1;
			}
		}
		std::cout<<"Raw Push:"<<(boost::posix_time::microsec_clock::local_time()-start).total_microseconds()<<" us" << " for "<<npushes<<" push"<<std::endl;
	} else {
		DataSet mydataset("myCU");
		double mydouble=0;
		int mint=0;
		int64_t mint64=0;
		std::string cs="ciao ciao";
		mydataset.add("doubleval",TYPE_DOUBLE,mydouble);
		mydataset.add("int32val",TYPE_INT32,mint);
		mydataset.add("int64val",TYPE_INT64,mint64);
		mydataset.add("stringval",TYPE_STRING,cs);
		cassandra.dropData(mydataset);
		boost::posix_time::ptime start=boost::posix_time::microsec_clock::local_time();
		for(int cnt=0;cnt<npushes;cnt++){
				mydouble+=2.0;
				mint++;
				mint64+=1000;
				cs+=".";
				mydataset.set("stringval",cs);
				if(cassandra.pushData(mydataset,0)!=0){
				  std::cerr<<"Error pushing"<<std::endl;
				  return -1;
				}
		}
				std::cout<<"Dataset Push:"<<(boost::posix_time::microsec_clock::local_time()-start).total_microseconds()<<" us" << " for "<<npushes<<" push"<<std::endl;
	}



  return 0;
}
