/*
 * DataSet.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */

#include "DataSet.h"
using namespace common::misc::data;

DataSet::DataSet() {
	// TODO Auto-generated constructor stub
	name="noname";
}

DataSet::~DataSet() {
	// TODO Auto-generated destructor stub
}

DataSet::DataSet(std::string _name):name(_name){

};


void* DataSet::add(const std::string& name,dataTypes type){
	if(elem_by_name.find(name)!=elem_by_name.end())
		return 0;
	DatasetElement_psh ds = boost::shared_ptr<DatasetElement>(new DatasetElement);

	ds->name =name;
	ds->type=type;
	switch(type){
			case (TYPE_INT32):
				 ds->size = 4;
				 ds->buffer = malloc(ds->size);

					break;
			case (TYPE_INT64):
				ds->size = 8;
				ds->buffer = malloc(ds->size);
				break;
			            //!Double 64 bit length
			case (TYPE_DOUBLE):
				ds->size = 8;
				ds->buffer = malloc(ds->size);
				break;

			case (TYPE_STRING):{

				break;
			}
			case (TYPE_BOOLEAN):
				ds->size = sizeof(bool);
				ds->buffer = malloc(ds->size);
				break;
			default:

				break;

		}
	elems.push_back(ds);
	elem_by_name[name]=ds;
	return ds->buffer;
}

void DatasetElement::resize(int siz){
	if(siz>size){
		buffer=realloc(buffer,siz);
		if(buffer==NULL){
			ERR("error allocating");
		}
		size = siz;
	}
}

void* DataSet::add(const std::string& name,dataTypes type,int size){
	if(elem_by_name.find(name)!=elem_by_name.end())
		return 0;
	DatasetElement_psh ds = boost::shared_ptr<DatasetElement>(new DatasetElement);
	DPRINT("adding element \"%s\" size %d",name.c_str(),size);
	ds->name =name;
	ds->type=type;
	ds->size = size;
	ds->buffer=(void*)malloc(size);
	elems.push_back(ds);
	elem_by_name[name]=ds;
	return ds->buffer;
}
int DataSet::set(int idx,void*ptr,int size){
	if(idx<elems.size()){
		elems[idx]->resize(size);
		memcpy(elems[idx]->buffer,ptr,size);
		return 0;
	}
	return -1;
}

int DataSet::set(const std::string& name,void*ptr,int size){
	if(elem_by_name.find(name)==elem_by_name.end())
			return -1;
	elem_by_name[name]->resize(size);
	memcpy(elem_by_name[name]->buffer,ptr,size);
	return 0;
}
