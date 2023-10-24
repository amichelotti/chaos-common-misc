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
using namespace ::common::misc::data;
using namespace std;

#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */


unsigned long ::common::misc::data::simpleHash(const char* s,int size)
{
  int cnt=0;
  size_t h = 31 /* also prime */;
   while(cnt<size){
     h = (h * A) ^ (s[cnt] * B);
     cnt++;
   }
   return h; // or return h % C;
}
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

DataSet::DataSet(const std::string& uid,const std::string& _name):name(_name),uuid(uid){
	std::replace(name.begin(),name.end(),'/','_');
	std::replace(uuid.begin(),uuid.end(),'/','_');

};

void* DataSet::addInt(const std::string& name,dataTypes type,void*pnt,int size,int inter){
	if(elem_by_name.find(name)!=elem_by_name.end())
	  return 0;

	DatasetElement_psh ds = std::shared_ptr<DatasetElement>(new DatasetElement);
	ds->size = size;
	ds->name =name;
	ds->type=type;
	if((inter==0)&&(type&0xff) == TYPE_STRING){

		std::string* a=(std::string*)pnt;
		ds->molteplicity=size/a->size();
		ds->buffer=(void*)a->c_str();

	} else {
		ds->molteplicity=size/type2size(type);
		ds->buffer=pnt;

	}
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
	std::map<std::string, DatasetElement_psh >::iterator i=elem_by_name.find(name);
	if(i!=elem_by_name.end()){
		int tt=type|(int)TYPE_ACCESS_ARRAY;
		i->second->type=(dataTypes)tt;
		i->second->size  +=type2size(type);
		i->second->buffer = realloc(pnt,size);
		i->second->molteplicity++;
		DPRINT("adding another element of \"%s\"[%d] type %d, tot size %d",name.c_str(),i->second->molteplicity,type,i->second->size);
		return i->second->buffer;
	} else {
		DPRINT("adding element \"%s\" with memory type %d",name.c_str(),type);
		size  =type2size(type);

	}

	pnt = malloc(size);
	return addInt(name,type,pnt, size,1);
	
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
    	  if(elems[idx]->buffer){
    		  free(elems[idx]->buffer);
    	  }

      }
      common::misc::data::dataTypes type =  elems[idx]->type;
      if((size > type2size(type))&&((size % type2size(type)) == 0)){
    	  int tt=elems[idx]->type|(int)TYPE_ACCESS_ARRAY;
          elems[idx]->type=(dataTypes)tt;
          elems[idx]->molteplicity=size / type2size(type);

      }

	  elems[idx]->buffer=ptr;
	  elems[idx]->size=size;

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
	ss<<"{\"" <<ds.name<<"\":";
	if(ds.type&TYPE_ACCESS_ARRAY){
				ss<<"[";
	}
	for(int cnt=0;cnt<ds.molteplicity;cnt++){

	switch(ds.type&0xFF){
		case (TYPE_INT32):
		ss <<*(int32_t*)ds.buffer;


		  break;
		case (TYPE_INT64):
		 ss<<*(int64_t*)ds.buffer;

		  break;
		  //!Double 64 bit length
		case (TYPE_DOUBLE):
		ss<<*(double*)ds.buffer;

		  break;

		case (TYPE_STRING):{
			std::string tmp((const char*)ds.buffer);
			ss<<"\""<<tmp<<"\"";

		  break;
		}
		case (TYPE_BOOLEAN):
		ss<<*(bool*)ds.buffer;

		  break;
		default:

		  break;

		}
	if((ds.type&TYPE_ACCESS_ARRAY)&&(cnt+1)<ds.molteplicity){
		ss<<",";
	}
	}
	if(ds.type&TYPE_ACCESS_ARRAY){
					ss<<"]";
	}
	ss<<"}";
	return in<<ss.str();
}
}}}


