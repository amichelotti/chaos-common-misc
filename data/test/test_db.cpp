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
#include <common/misc/data/core/DBbaseFactory.h>
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
	std::string replication;
	std::string dbtype;
	int enable_raw=0;
	int n_threads=1;
	po::options_description desc("options");

	desc.add_options()("help","help");
	desc.add_options()("server,s",po::value<std::vector<std::string> >(&servers),"contacting servers");
	desc.add_options()("pushes,p",po::value<int >(&npushes)->default_value(NPUSHES),"number of pushes");
	desc.add_options()("dbname,n",po::value<std::string >(&dbname)->default_value("chaos"),"db name");
	desc.add_options()("threads,t",po::value<int >(&n_threads)->default_value(1),"number of threads ");
	desc.add_options()("replication,r",po::value<std::string >(&replication)->default_value("1"),"replication factor (1= just one server keep data)");
	desc.add_options()("dbtype",po::value<std::string >(&dbtype)->default_value("cassandra"),"DB type to use");

	desc.add_options()("dbtable,d",po::value<std::string >(&dbtable),"db table enable raw access mode");
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
	DBbase* db=DBbaseFactory::getInstance(dbtype,dbname);
	if(db == NULL){
		ERR("cannot retrieve database %s",dbtype.c_str());
		return -1;
	}
	db->addDBServer(servers);
	{
		std::stringstream ss;
		ss<<n_threads;
		db->setDBParameters("threads",ss.str());
	}


	db->setDBParameters("replication",replication);

	if(db->connect()!=0){
		ERR("error connecting");
		return -1;
	}
	if(enable_raw){
		//////////// PUSH
		boost::posix_time::ptime start=boost::posix_time::microsec_clock::local_time();
		for(int cnt=0;cnt<npushes;cnt++){
			std::stringstream ss;
			ss<<"{\"prova\":"<<cnt<<"}";
			std::string ds=ss.str();
			if(db->pushData(dbtable,key,ds,0)!=0){
			  std::cerr<<"Error pushing"<<std::endl;
			  return -1;
			}
		}
		uint64_t tot=(boost::posix_time::microsec_clock::local_time()-start).total_microseconds();

		std::cout<<"Raw Push:"<<tot<<" us" << " for "<<npushes<<" push,"<<" rate:"<<(npushes*1000000)/tot<<" psuh/s"<<std::endl;
		////READ
		{
			blobRecord_t ret;

			start=boost::posix_time::microsec_clock::local_time();
			if(db->queryData(dbtable,key,ret)!=0){
				std::cerr<<"Error retriving data"<<std::endl;
				return -1;
			}
			tot=(boost::posix_time::microsec_clock::local_time()-start).total_microseconds();
			std::cout<<"Raw Query with key:"<<tot<<" us" << " for "<<ret.size()<<" push,"<<" rate:"<<(ret.size()*1000000)/tot<<" push/s"<<std::endl;
			for(blobRecord_t::iterator i=ret.begin();i!=ret.end();i++){
				std::cout<<"["<<i->timestamp<<"] "<<(i->key)<<":"<<(i->data)<<std::endl;
			}
		}


		{
					blobRecord_t ret;

					start=boost::posix_time::microsec_clock::local_time();
					if(db->queryData(dbtable,"",ret)!=0){
						std::cerr<<"Error retrieving data"<<std::endl;
						return -1;
					}
					tot=(boost::posix_time::microsec_clock::local_time()-start).total_microseconds();
					std::cout<<"Raw Query without key:"<<tot<<" us" << " for "<<ret.size()<<" push,"<<" rate:"<<(ret.size()*1000000)/tot<<" push/s"<<std::endl;
					for(blobRecord_t::iterator i=ret.begin();i!=ret.end();i++){
						std::cout<<"["<<i->timestamp<<"] "<<(i->key)<<":"<<(i->data)<<std::endl;
					}
		}

	} else {
		DataSet mydataset("typeTest","myCU");
		double mydouble=0;
		int mint=0;
		int64_t mint64=0;
		std::string cs="ciao ciao";
		double array[10];
		mydataset.add("doubleval",TYPE_DOUBLE,mydouble);
		mydataset.add("int32val",TYPE_INT32,mint);
		mydataset.add("int64val",TYPE_INT64,mint64);
		mydataset.add("stringval",TYPE_STRING,cs);
		mydataset.add("myarray",TYPE_DOUBLE|TYPE_ACCESS_ARRAY,array,sizeof(array));

		db->dropData(mydataset);
		boost::posix_time::ptime start=boost::posix_time::microsec_clock::local_time();
		for(int cnt=0;cnt<npushes;cnt++){
				mydouble+=2.0;
				mint++;
				mint64+=1000;
				cs+=".";
				for(int cntt=0;cntt<sizeof(array)/sizeof(double);cntt++){
					array[cntt]=cnt+1000*cntt;
				}
				mydataset.set("stringval",cs);
				if(db->pushData(mydataset,0)!=0){
				  std::cerr<<"Error pushing"<<std::endl;
				  return -1;
				}
		}
				uint64_t tot=(boost::posix_time::microsec_clock::local_time()-start).total_microseconds();
				std::cout<<"Dataset Push:"<<tot<<" us" << " for "<<npushes<<" push, rate:"<<(npushes*1000000)/tot<<" push/s"<<std::endl;


				datasetRecord_t ret,rlast,rfirst;
				 start=boost::posix_time::microsec_clock::local_time();

				//get all
				if(db->queryData(mydataset,ret)!=0){
					std::cerr<<"error retriving dataset";
					return -1;
				}
				tot=(boost::posix_time::microsec_clock::local_time()-start).total_microseconds();
				std::cout<<"Dataset Get:"<<tot<<" us" << " for "<<npushes<<" push, rate:"<<(tot*npushes)/1000000<<" Hz"<<std::endl;

				for(datasetRecord_t::iterator i=ret.begin();i!=ret.end();i++){
					std::cout<<"["<<i->first<<"] "<<i->second<<std::endl;
				}
				//get last

				//get last
				if(db->queryData(mydataset,rlast,-1,-1)!=0){
					std::cerr<<"error retriving dataset";
					return -1;
				}
				std::cout<<"LAST"<<std::endl;
				for(datasetRecord_t::iterator i=rlast.begin();i!=rlast.end();i++){
									std::cout<<"["<<i->first<<"] "<<i->second<<std::endl;
			}

				//get first
								if(db->queryData(mydataset,rfirst,0,0)!=0){
									std::cerr<<"error retriving dataset";
									return -1;
								}
								std::cout<<"FIRST"<<std::endl;
								for(datasetRecord_t::iterator i=rfirst.begin();i!=rfirst.end();i++){
													std::cout<<"["<<i->first<<"] "<<i->second<<std::endl;
							}

	}



  return 0;
}
