/*
 * DataSet.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */
#undef DEBUG
#include "DataSet.h"
#include <string.h>
#include <sstream>
#include <boost/date_time.hpp>
using namespace common::misc::data;

DataSet::DataSet() {
	// TODO Auto-generated constructor stub
	name="noname";
	uuid="nover";
	DPRINT("creating dataset %d",elems.size());
}

DataSet::~DataSet() {
	// TODO Auto-generated destructor stub
 // DPRINT("destroying %s",name.c_str());
}

DataSet::DataSet(std::string uid,std::string _name):name(_name),uuid(uid){
};

void* DataSet::add(const std::string& name,dataTypes type,void*pnt,int size,int inter){
	if(elem_by_name.find(name)!=elem_by_name.end())
	  return 0;

	DatasetElement_psh ds = boost::shared_ptr<DatasetElement>(new DatasetElement);
	ds->buffer=pnt;
	ds->size = size;
	ds->name =name;
	ds->type=type;
	ds->molteplicity=size/type2size(type);
	ds->internal+=inter;
	DPRINT("adding element \"%s\" type %d, pnt 0x%x size %d, internal %d,molteplicity %d",name.c_str(),type,pnt,size,ds->internal,ds->molteplicity);
	elems.push_back(ds);
	elem_by_name[name]=ds;
	return ds->buffer;

}
 const char* DataSet::getTag() const {
	std::stringstream ss;
	boost::posix_time::ptime pt=boost::posix_time::second_clock::local_time();
	ss<<pt.date();
	return ss.str().c_str();
}
 int DataSet::type2size(dataTypes type){
	 switch(type&0xff){
	 	case (TYPE_INT32):
	 	  return sizeof(int);

	 	case (TYPE_INT64):
	 	  return sizeof(int64_t);

	 	  //!Double 64 bit length
	 	case (TYPE_DOUBLE):
	 	 return sizeof(double);

	 	case (TYPE_STRING):
	 			return 1024;
	 	case (TYPE_BOOLEAN):
	 	 return sizeof(bool);


	 	}
		 return sizeof(double);

 }
void* DataSet::add(const std::string& name,dataTypes type){
	int size=8;
	void *pnt=NULL;
	if(elem_by_name.find(name)!=elem_by_name.end()){
		DPRINT("adding another element of type %d",type);
		int tt=type;
		tt+=(int)TYPE_ACCESS_ARRAY;
		type =  (dataTypes)tt;
		size  +=type2size(type);
	} else {
		DPRINT("adding element \"%s\" with memory type %d",name.c_str(),type);
		size  =type2size(type);
	}

	pnt = realloc(pnt,size);
	return add(name,type,pnt, size,1);
	
}

void DatasetElement::resize(int siz){
	if(siz>size){
		buffer=realloc(buffer,siz);
		internal++;
		if(buffer==NULL){
			ERR("error allocating");
		}
		size = siz;
	}
}
/*
void* DataSet::add(const std::string& name,dataTypes type,int size){
	if(elem_by_name.find(name)!=elem_by_name.end())
		return 0;
	DatasetElement_psh ds = boost::shared_ptr<DatasetElement>(new DatasetElement);
	DPRINT("adding element \"%s\" size %d",name.c_str(),size);
	ds->name =name;
	ds->type=type;
	ds->size = size;
	ds->buffer=(void*)malloc(size);
	ds->internal++;
	elems.push_back(ds);
	elem_by_name[name]=ds;
	return ds->buffer;
}
*/
int DataSet::set(int idx,void*ptr,int size){
    if(idx<elems.size()){
      if(elems[idx]->internal){
	elems[idx]->resize(size);
	memcpy(elems[idx]->buffer,ptr,size);

      } else {
	elems[idx]->buffer=ptr;
	elems[idx]->size=size;
      }
      return 0;
    }
    return -1;
}

int DataSet::set(const std::string& name,void*ptr,int size){
	if(elem_by_name.find(name)==elem_by_name.end())
			return -1;
	if(elem_by_name[name]->internal){
	  
	  elem_by_name[name]->resize(size);
	  memcpy(elem_by_name[name]->buffer,ptr,size);
	  
	} else {
	  elem_by_name[name]->buffer=ptr;
	  elem_by_name[name]->size=size;
	}
	return 0;
}

namespace common  {
namespace misc {
namespace data {
std::ostream& operator<<(std::ostream& in,const DataSet&d){
	std::vector<DataSet::DatasetElement_psh> ds=d.elems;
	in<<"{\"type\":"<<d.uuid<<",\"name\":"<<d.name<<",\"values\":[";
	for(std::vector<DataSet::DatasetElement_psh>::iterator i= ds.begin();i!=ds.end();i++){
		const DatasetElement& k=*(*i);
		if(i+1 == ds.end()){
			in<<k;
		} else {
			in<<k<<",";
		}
	}
	in<<"]}";
	return in;
}
std::ostream& operator<<(std::ostream& in,const DatasetElement&ds){
	std::stringstream ss;
	switch(ds.type){
		case (TYPE_INT32):
		  ss<<"{\""<<ds.name<<"\":"<<*(int32_t*)ds.buffer<<"}";


		  break;
		case (TYPE_INT64):
		 ss<<"{\""<<ds.name<<"\":"<<*(int64_t*)ds.buffer<<"}";

		  break;
		  //!Double 64 bit length
		case (TYPE_DOUBLE):
		ss<<"{\""<<ds.name<<"\":"<<*(double*)ds.buffer<<"}";

		  break;

		case (TYPE_STRING):{
			std::string tmp((const char*)ds.buffer);
			ss<<"{\""<<ds.name<<"\": \""<<tmp<<"\"}";

		  break;
		}
		case (TYPE_BOOLEAN):
		ss<<"{\""<<ds.name<<"\":"<<*(bool*)ds.buffer<<"}";

		  break;
		default:

		  break;

		}
	return in<<ss.str();
}
}}}


