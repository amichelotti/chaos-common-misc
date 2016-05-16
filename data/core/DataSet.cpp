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


int DataSet::add(const std::string& name,int type){
	if(elem_by_name.find(name)!=elem_by_name.end())
		return -1;
	dataset_ele* ptr= new dataset_ele();
	ptr->name =name;
	ptr->type=type;
	elems.push_back(ptr);
	elem_by_name[name]=ptr;
	return 0;
}
int DataSet::set(int idx,void*ptr,int size){
	if(idx<elems.size()){
		elems[idx]->buffer = ptr;
		elems[idx]->size = size;
		return 0;
	}
	return -1;
}

int DataSet::set(const std::string& name,void*ptr,int size){
	if(elem_by_name.find(name)==elem_by_name.end())
			return -1;
	elem_by_name[name]->buffer = ptr;
	elem_by_name[name]->size = size;
	return 0;
}
