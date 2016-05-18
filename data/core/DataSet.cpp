/*
 * DataSet.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */

#include "DataSet.h"
#include <string.h>
using namespace common::misc::data;

DataSet::DataSet() {
	// TODO Auto-generated constructor stub
	name="noname";
}

DataSet::~DataSet() {
	// TODO Auto-generated destructor stub
  DPRINT("destroying %s",name.c_str());
}

DataSet::DataSet(std::string _name):name(_name){

};

void* DataSet::add(const std::string& name,dataTypes type,void*pnt,int size,int inter){
	if(elem_by_name.find(name)!=elem_by_name.end())
	  return 0;
	DPRINT("adding element \"%s\" type %d, pnt 0x%x size %d, internal %d",name.c_str(),type,pnt,size,inter);
	DatasetElement_psh ds = boost::shared_ptr<DatasetElement>(new DatasetElement);
	ds->buffer=pnt;
	ds->size = size;
	ds->name =name;
	ds->type=type;
	ds->internal+=inter;
	elems.push_back(ds);
	elem_by_name[name]=ds;
	return ds->buffer;

}

void* DataSet::add(const std::string& name,dataTypes type){
	int size=8;
	void *pnt=NULL;
	DPRINT("adding element \"%s\" with memory type %d",name.c_str(),type);
	switch(type){
	case (TYPE_INT32):
	  size = 4;
	  
	  
	  break;
	case (TYPE_INT64):
	  size = 8;
	  
	  break;
	  //!Double 64 bit length
	case (TYPE_DOUBLE):
	  size = 8;
	  
	  break;
	  
	case (TYPE_STRING):{
	  size = 1024;
	  
	  break;
	}
	case (TYPE_BOOLEAN):
	  size = sizeof(bool);
	  break;
	default:
	  
	  break;
	  
	}
	pnt = malloc(size);
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
