/*
 * DBCassandra.cpp
 *
 *  Created on: Feb 24, 2016
 *      Author: michelo
 */
#undef DEBUG
#include "DBCassandra.h"
#include <sstream>
#include <common/debug/core/debug.h>
#include <boost/date_time.hpp>
namespace common {
namespace misc {
namespace data {
  static int64_t epoch_ms(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    int64_t mslong = (long long) tp.tv_sec * 1000L + tp.tv_usec / 1000; //get current timestamp in milliseconds
    return mslong;
  }
DBCassandra::DBCassandra(const std::string name):DBbase<DBCassandra>(name){
	// TODO Auto-generated constructor stub
	cluster=NULL;
	session=NULL;
	is_connected = false;
	n_threads=1;
	future_answer=NULL;
	curr_answer=0;
	replication_factor="1";
	counter =0;

}

DBCassandra::~DBCassandra() {
	// TODO Auto-generated destructor stub
	disconnect();
}

#define TYPE2CASSANDRA(type,native) if(type&TYPE_ACCESS_ARRAY) return "list<" native ">"; else return native;

const char* DBCassandra::dataTypeToCassandra(dataTypes t){

	switch(t&0xff){
	case (TYPE_INT32):
		TYPE2CASSANDRA(t,"int");

	case (TYPE_INT64):
		TYPE2CASSANDRA(t,"bigint");
	            //!Double 64 bit length
	case (TYPE_DOUBLE):
		TYPE2CASSANDRA(t,"double");

	case (TYPE_STRING):
		TYPE2CASSANDRA(t,"text");

	case (TYPE_BYTEARRAY):
		TYPE2CASSANDRA(t,"varchar");

	case (TYPE_BOOLEAN):
		TYPE2CASSANDRA(t,"boolean");

	case (TYPE_CLUSTER):
				TYPE2CASSANDRA(t,"varchar");
	default :
		ERR("undefined type");
		TYPE2CASSANDRA(t,"varchar");
	}
}

void DBCassandra::print_error(CassFuture* future) {
  const char* message;
  size_t message_length;
  cass_future_error_message(future, &message, &message_length);
  ERR("Error: %.*s", (int)message_length, message);
}
CassError DBCassandra::waitAll(){
	CassError rc=CASS_OK;
	for(int cnt=0;(future_answer!=NULL) && (cnt<n_threads);cnt++){
			if(future_answer[cnt]!=NULL){
				cass_future_wait(future_answer[cnt]);
				rc = cass_future_error_code(future_answer[cnt]);
				if (rc != CASS_OK) {
							  print_error(future_answer[cnt]);
				}
				cass_future_free(future_answer[cnt]);
				future_answer[cnt]=NULL;
			}
		}
	return rc;

}
CassError DBCassandra::execStatement(CassStatement* statement,bool wait){
	CassError rc = CASS_OK;
	CassFuture* future ;
	if(wait){
		waitAll();
	}

	if(wait){
		future = cass_session_execute(session, statement);

		cass_future_wait(future);
		rc = cass_future_error_code(future);
		if (rc != CASS_OK) {
			  print_error(future);
		}

		cass_future_free(future);

	} else {
		int off=curr_answer%n_threads;
		if(curr_answer>=n_threads){
			if(cass_future_ready(future_answer[off])==false){
				DPRINT("waiting end request %d-%d id 0x%x",curr_answer,off,future_answer[off]);
				cass_future_wait(future_answer[off]);
			}
			rc = cass_future_error_code(future_answer[off]);
			if (rc != CASS_OK) {
					  print_error(future_answer[off]);
			}
			cass_future_free(future_answer[off]);

		}
		future = cass_session_execute(session, statement);
		DPRINT("executing statement, id batch %d-%d, id 0x%x",curr_answer,off,future)

		future_answer[off]= future;

		curr_answer++;

	}
	cass_statement_free(statement);
	return rc;
}

int DBCassandra::executeQuery(const std::string& query){
	CassError rc = CASS_OK;
	DPRINT("executing statement \"%s\"",query.c_str());
	CassStatement* statement = cass_statement_new(query.c_str(), 0);
	rc=execStatement(statement);

	  if(rc==CASS_OK)
		  return 0;
	  DERR("error executing statement \"%s\"",query.c_str());

	  return  rc==CASS_OK?0:(int)rc;;
}
int DBCassandra::connect(){
	if(name.empty()){
		ERR("DB name cannot be empty");
		return -1120;

	}

	if(is_connected==true){
		ERR("already connected");
		return 0;
	}
	if(kv_parameters.find("threads")!=kv_parameters.end()){
		int th=atoi(kv_parameters["threads"].c_str());
		if(th>1){

			n_threads=th;
		}
	}
	if(future_answer==NULL){
		DPRINT("allocating %d futures",n_threads);
		future_answer = (CassFuture**) calloc(n_threads,sizeof(CassFuture*));

	}
	if(kv_parameters.find("replication")!=kv_parameters.end()){
		replication_factor=kv_parameters["replication"];
	}
	cluster = cass_cluster_new();
	if(cluster == NULL){
	  ERR("cannot create cluster structure");
	  return -100;
	}
	session = cass_session_new();
	if(session == NULL){
	  ERR("cannot create new session");
	  return -101;
	}
	if(servers.empty()){
		DERR("no servers are specified");
		return -1;
	}
	if(cluster==NULL){
		DERR("no cluster created");
		return -1;
	}
	for(std::vector <std::string>::iterator i=servers.begin();i!=servers.end();i++){
	  DPRINT("setting new contact point \"%s\"",i->c_str());
	  cass_cluster_set_contact_points(cluster, i->c_str());

	}

	 CassError rc = CASS_OK;
	 int rcc=-4;
	 CassFuture* future = cass_session_connect(session, cluster);

	  cass_future_wait(future);
	  rc = cass_future_error_code(future);
	  if (rc != CASS_OK) {
	    print_error(future);
	  }
	  cass_future_free(future);

	  if(rc==CASS_OK){
	    std::stringstream ss;
	    ss<<"CREATE KEYSPACE IF NOT EXISTS "<<name<<" WITH replication = { 'class': 'SimpleStrategy', 'replication_factor': '"<<replication_factor<<"' };";
	    rcc=executeQuery(ss.str().c_str());
	    if(rcc!=0){
	      ERR("error creating KEYSPACE %s",name.c_str());
	    }
	  }
	  if(rcc==0){
		  is_connected = true;
	  }
	  return rcc;

}
int DBCassandra::pushData(const DataSet& dset,uint64_t ts){
	CassError rc = CASS_OK;
	int rcc;
	CassFuture* future = NULL;
	std::stringstream query,params,table_query;
	const CassPrepared* prep;

	std::vector<DataSet::DatasetElement_psh> elems=dset.getDataSet();

	std::string table_name= name + "."+dset.getUID();
	if(prepared.find(dset.getName())!=prepared.end()){
		prep=prepared[dset.getName()];

	} else {
		table_query <<"CREATE TABLE IF NOT EXISTS "<<table_name<<" (uuid text,event_time timestamp,";
		query<<"INSERT INTO "<<table_name<<" (uuid,event_time,";

		params<<" VALUES (?,?,";
		for(std::vector<DataSet::DatasetElement_psh>::iterator i=elems.begin();i!=elems.end();i++){
			if((i+1)!=elems.end()){
				query<<(*i)->name<<",";
				params<<"?,";
			} else {
				query<<(*i)->name<<")";
				params<<"?);";
			}
			table_query<<(*i)->name<<" "<<dataTypeToCassandra((*i)->type)<<",";

		}
		query<<params.str();
		table_query<<" PRIMARY KEY (uuid,event_time));";
		rcc= executeQuery(table_query.str());
		if(rcc!= 0){
			DERR("error creating table ");
			return rcc;
		}

		DPRINT("prepared query created for \"%s\" = \"%s\"",dset.getName().c_str(),query.str().c_str());
		future = cass_session_prepare(session, query.str().c_str());
		cass_future_wait(future);
		rc = cass_future_error_code(future);
		if (rc != CASS_OK) {
			    print_error(future);
			    return -1;
		} else {
			    prep = cass_future_get_prepared(future);
		}

		cass_future_free(future);

		prepared[dset.getName()]=prep;
	}
	CassStatement* statement = NULL;
	int cnt=1;
	statement = cass_prepared_bind(prep);
	if(ts==0){
	  ts=epoch_ms();
	}
	//	DPRINT("pushing %lld",ts);
	cass_statement_bind_string(statement, 0, dset.getName().c_str());
	cass_statement_bind_int64(statement, 1, (int64_t)ts + counter++);
	cnt=2;
	CassCollection* collection = NULL;
	for(std::vector<DataSet::DatasetElement_psh>::iterator i=elems.begin();i!=elems.end();i++,cnt++){
		DPRINT("[%d] binding %s %s[%d] size %d, of type %d @0x%x",cnt,((*i)->type & TYPE_ACCESS_ARRAY)?"array":"scalar",(*i)->name.c_str(),(*i)->molteplicity,(*i)->size,(*i)->type&0xff,(*i)->buffer);
		if((*i)->type & TYPE_ACCESS_ARRAY){
			void* ptr=(*i)->buffer;
			collection = cass_collection_new(CASS_COLLECTION_TYPE_LIST, (*i)->molteplicity);

			for(int cntt=0;cntt<(*i)->molteplicity;cntt++){

				switch((*i)->type&0xff){
					case (TYPE_INT32):{
							int32_t* val=(int32_t*)ptr;
						   cass_collection_append_int32(collection, val[cntt]);
						   break;
					}
					case (TYPE_INT64):{
							int64_t* val=(int64_t*)ptr;
						   cass_collection_append_int64(collection, val[cntt]);
						   break;
					}
					case (TYPE_DOUBLE):{
												double* val=(double*)ptr;
											   cass_collection_append_double(collection, val[cntt]);
											   break;
										}

					default:
						ERR("type array not supported %s %d",(*i)->name.c_str(),(*i)->type);
						return -110;
				}



			}

			cass_statement_bind_collection_by_name(statement, (*i)->name.c_str(), collection);
			cass_collection_free(collection);
		} else {
			switch((*i)->type){
				case (TYPE_INT32):

					  rc=cass_statement_bind_int32_by_name(statement, (*i)->name.c_str(), (int32_t)**i);
						break;
				case (TYPE_INT64):
					rc=cass_statement_bind_int64_by_name(statement, (*i)->name.c_str(), (int64_t)**i);
					break;
							//!Double 64 bit length
				case (TYPE_DOUBLE):
					rc=cass_statement_bind_double_by_name(statement, (*i)->name.c_str(), (double)**i);
					break;

				/*case (TYPE_STRING):{
					rc=cass_statement_bind_string_by_name_n(statement, (*i)->name.c_str(), (const char*)(*i)->buffer,(*i)->size);
					break;
				}*/
				case (TYPE_BOOLEAN):
					rc=cass_statement_bind_bool_by_name(statement, (*i)->name.c_str(), (cass_bool_t)**i);
					break;

				default:
					rc=cass_statement_bind_string_by_name_n(statement, (*i)->name.c_str(), (*i)->name.size(),(const char*)(*i)->buffer,(*i)->size);
				break;

			}
		}
		if(rc!=CASS_OK){
		  ERR("error binding [%d] \"%s\" type %d",cnt,(*i)->name.c_str(),(*i)->type);
		  cass_statement_free(statement);
		  return rc;

		}
	}
	rc= execStatement(statement,false);
	return rc;
}
int DBCassandra::pushData(const std::string &tbl,const std::string &key,std::string& ds,uint64_t ts){
	CassError rc = CASS_OK;
	int rcc;
	CassFuture* future = NULL;
	std::stringstream query,table_query;
	const CassPrepared* prep;

	
	if(prepared.find(tbl)!=prepared.end()){
			prep=prepared[tbl];

	} else {
	  table_query <<"CREATE TABLE IF NOT EXISTS "<<name<<"."<<tbl<<" (uuid text, event_time timestamp, data varchar,PRIMARY KEY (uuid,event_time));";
		rcc= executeQuery(table_query.str());
		if(rcc!= 0){
			DERR("error creating table ");
			return rcc;
		}

		query<<"INSERT INTO "<<name<<"."<<tbl<<" (uuid,event_time,data) VALUES (?,?,?);";


		DPRINT("prepared query created for \"%s\" = \"%s\"",tbl.c_str(),query.str().c_str());
		future = cass_session_prepare(session, query.str().c_str());
		cass_future_wait(future);
		rc = cass_future_error_code(future);
		if (rc != CASS_OK) {
			cass_future_free(future);

			print_error(future);
			return -1;
		} else {
		  prep = cass_future_get_prepared(future);
		}

		cass_future_free(future);

		prepared[tbl]=prep;
	}

	CassStatement* statement = NULL;
	int cnt=1;
	statement = cass_prepared_bind(prep);
	if(ts==0){
		ts=epoch_ms();
	}
	//	DPRINT("pushing %lld",ts);
	cass_statement_bind_string(statement, 0, key.c_str());
	cass_statement_bind_int64(statement, 1, (int64_t)ts);
	cass_statement_bind_string_n(statement, 2, ds.c_str(),ds.size());
	rc= execStatement(statement,true);
	return rc;

}
int DBCassandra::queryData(const DataSet& ds,datasetRecord_t& set ,int64_t startTime,int64_t endTime){
	std::stringstream query;
	CassError rc;
	int rcc;
	std::string table_name= name + "."+ds.getUID();

	std::vector<DataSet::DatasetElement_psh> elems=ds.getDataSet();

	if(startTime==0 && endTime==-1){
		query<<"SELECT * from "<<table_name<<" where uuid='"<<ds.getName()<<"' order by event_time desc;";
	} else if(startTime==-1 && endTime ==-1){
		query<<"SELECT * from "<<table_name<<" where uuid='"<<ds.getName()<<"' order by event_time desc limit 1;";
	} else if(startTime==0 && endTime ==0){
		query<<"SELECT * from "<<table_name<<" where uuid='"<<ds.getName()<<"' order by event_time asc limit 1;";
	} else {
		query<<"SELECT * from "<<table_name<<" where uuid='"<<ds.getName()<<"' AND event_time >="<<startTime<<" AND event_time <="<< endTime<<" order by event_time desc ;";
	}
	DPRINT("query: \"%s\"",query.str().c_str());
	CassStatement* statement = cass_statement_new(query.str().c_str(), 0);
	CassFuture* future = cass_session_execute(session, statement);
	rc = cass_future_error_code(future);
	if (rc != CASS_OK) {
		print_error(future);
		cass_future_free(future);

		return -1;
	} else {
		 const CassResult* result = cass_future_get_result(future);

		  if (result == NULL) {
		     /* Handle error */
			ERR("no result from query: \"%s\"",query.str().c_str());
		     cass_future_free(future);
		     cass_statement_free(statement);
		     return -2;
		  }
		  CassIterator* iterator = cass_iterator_from_result(result);
		  int nrows=0,cnt;
		  while (cass_iterator_next(iterator)) {
		        const CassRow* row = cass_iterator_get_row(iterator);
		        int64_t ts;
		        cnt=1;
		        const CassValue*col_value=cass_row_get_column(row, cnt);
		        cass_value_get_int64(col_value,(int64_t*)&ts);
		        cnt++;
		        DataSet tmp("result","res");
		        for(std::vector<DataSet::DatasetElement_psh>::iterator i=elems.begin();i!=elems.end();i++,cnt++){
		        	CassIterator * items_iterator;
		        	int cnti=0;
		        	col_value=cass_row_get_column_by_name(row, (*i)->name.c_str());

        			DPRINT("accessing[%d] %s of type %d, name %s[%d] cass type:0x%x",cnt,((*i)->type & TYPE_ACCESS_ARRAY)?"array":"scalar",(*i)->type&0xff, (*i)->name.c_str(),(*i)->molteplicity,cass_value_type(col_value));

		        	if((*i)->type & TYPE_ACCESS_ARRAY){
		        				items_iterator = cass_iterator_from_collection(col_value);

		        				 while (cass_iterator_next(items_iterator)) {
		        					 DPRINT("iterating %d,%d",cnt,cnti++);
		        					 switch((*i)->type&0xff){
		        					 case (TYPE_INT32):{
		        					 		     void* ptr=tmp.add((*i)->name,(*i)->type);
		        					 		     cass_value_get_int32(cass_iterator_get_value(items_iterator),(int32_t*)ptr);
		        					 			break;
		        					}

									case (TYPE_INT64):{

										void* ptr=tmp.add((*i)->name,(*i)->type);
										cass_value_get_int64(cass_iterator_get_value(items_iterator),(int64_t*)ptr);
										break;
									}
		        					 		        				            //!Double 64 bit length
									case (TYPE_DOUBLE):{
											void* ptr=tmp.add((*i)->name,(*i)->type);
											cass_value_get_double(cass_iterator_get_value(items_iterator),(double*)ptr);
											break;
										}
									default:
		        							ERR("type array not supported %s %d",(*i)->name.c_str(),(*i)->type);
		        							return -123;
		        					 }

		        				 }
		        				 cass_iterator_free(items_iterator);
		        			} else {
		        				switch((*i)->type){
		        				case (TYPE_INT32):{
		        					void* ptr=tmp.add((*i)->name,(*i)->type);
									cass_value_get_int32(col_value,(int32_t*)ptr);
									//tmp.add((*i)->name,(*i)->type,t);
									break;
		        				}

		        				case (TYPE_INT64):{

		        					void* ptr=tmp.add((*i)->name,(*i)->type);
									cass_value_get_int64(col_value,(int64_t*)ptr);
									break;
		        				}
		        				            //!Double 64 bit length
		        				case (TYPE_DOUBLE):{
			        					void* ptr=tmp.add((*i)->name,(*i)->type);
										cass_value_get_double(col_value,(double*)ptr);
										break;
			        				}

		        				case (TYPE_BOOLEAN):{
		        					void* ptr=tmp.add((*i)->name,(*i)->type);
		        					cass_value_get_bool(col_value,(cass_bool_t*)ptr);
		        					break;
		        				}


		        				default:{
		        					const char *str;
		        					size_t siz;
									cass_value_get_string(col_value,&str,&siz);
									tmp.add((*i)->name,(*i)->type);
									tmp.set((*i)->name,(void*)str,siz);
		        					break;
		        				}
		        			}
		        }
		        }
		        set.insert(std::make_pair<int64_t,DataSet>(ts,tmp));
		        //std::stringstream pp;
		       // pp<<"temp:"<<tmp<<" SET:"<<set[ts];
		        //DPRINT("%d] %s",nrows,pp.str().c_str());
		        nrows++;


		      }
		  	  cass_iterator_free(iterator);


	}
	cass_future_free(future);
	cass_statement_free(statement);
	if(rc!=CASS_OK){
		ERR("error executing query data");
		return -1;
	}
	return 0;
}
int DBCassandra::dropData(const DataSet& dset){
  std::stringstream ss;
  std::string table_name= name + "."+dset.getUID();

  DPRINT("Dropping table %s",table_name.c_str());
  ss<<"DROP TABLE "<<table_name<<";";
  int rcc=executeQuery(ss.str().c_str());
  return rcc;

}
int DBCassandra::disconnect(){
  DPRINT("disconnecting");
	if(session){
		DPRINT("closing session");
		CassFuture*  close_future = cass_session_close(session);
		cass_future_wait(close_future);
		cass_future_free(close_future);
	}
	if(cluster){
		cass_cluster_free(cluster);
		cluster=NULL;
		DPRINT("close cluster");
	}
	if(session){
	 cass_session_free(session);
	 session=NULL;
	 DPRINT("close session");
	}
	is_connected = false;
	waitAll();
	if(future_answer!=NULL){
			DPRINT("deallocating %d futures",n_threads);
			free (future_answer);
			future_answer=NULL;
		}

}
int DBCassandra::queryData(const std::string& tbl,const std::string& key,blobRecord_t& set,int64_t startTime,int64_t endTime){
	std::stringstream query,opt;
	CassError rc;
	int rcc;
	if(key.empty()){
		query<<"SELECT * from "<<name<<"."<<tbl<<";";
	} else {
	if(startTime==0 && endTime==-1){
			query<<"SELECT * from "<<name<<"."<<tbl<<" WHERE uuid = '"<<key<<"'"<<" order by event_time desc;";
		} else if(startTime==-1 && endTime ==-1){
			query<<"SELECT * from "<<name<<"."<<tbl<<" WHERE uuid = '"<<key<<"'"<<" order by event_time desc limit 1;";
		} else if(startTime==0 && endTime ==0){
			query<<"SELECT * from "<<name<<"."<<tbl<<" WHERE uuid = '"<<key<<"'"<<" order by event_time asc limit 1;";
		} else {
			if(key.empty()){
				query<<"SELECT * from "<<name<<"."<<" WHERE uuid = '"<<key<<"'"<<" where event_time >="<<startTime<<" AND event_time <="<< endTime<<" order by event_time desc ;";
			} else {
				query<<"SELECT * from "<<name<<"."<<tbl<<" where event_time >="<<startTime<<" AND event_time <="<< endTime<< "AND uuid = '"<<key<<"' order by event_time desc ;";
			}
		}
	}
	DPRINT("query:\"%s\"",query.str().c_str());
	CassStatement* statement = cass_statement_new(query.str().c_str(), 0);
	CassFuture* future = cass_session_execute(session, statement);
	rc = cass_future_error_code(future);
		if (rc != CASS_OK) {
			cass_future_free(future);
			cass_statement_free(statement);
			print_error(future);
			return -1;
		} else {
			 const CassResult* result = cass_future_get_result(future);

			  if (result == NULL) {
			     /* Handle error */
				ERR("no result from query: \"%s\"",query.str().c_str());
			     cass_future_free(future);
			     return -2;
			  }
			  CassIterator* iterator = cass_iterator_from_result(result);
			  int nrows=0,cnt;
			  while (cass_iterator_next(iterator)) {
			        const CassRow* row = cass_iterator_get_row(iterator);
			        int64_t ts;
			      	cass_value_get_int64(cass_row_get_column(row, 1),(int64_t*)&ts);
			      	const char *keys,*data;
			      	size_t sizkey,sizdata;
			      	cass_value_get_string(cass_row_get_column(row, 0),&keys,&sizkey);
			      	cass_value_get_string(cass_row_get_column(row, 2),&data,&sizdata);
			      	dbrecord_t t;
			      //	DPRINT("[%lld] %s %s",ts,key,data);
			      	t.timestamp = ts;
			      	t.key = keys;
			      	t.data = data;

			      	set.push_back(t);


			  }
			  cass_iterator_free(iterator);
		}

	cass_future_free(future);
	cass_statement_free(statement);


	return 0;
}


} /* namespace data */
} /* namespace misc */
} /* namespace common */
