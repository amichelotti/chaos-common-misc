/*
 * DataSet.h
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */

#ifndef DATA_CORE_DATASET_H_
#define DATA_CORE_DATASET_H_
#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#include <common/debug/core/debug.h>
#include <boost/shared_ptr.hpp>

namespace common  {
namespace misc {
namespace data {

enum dataTypes {
            TYPE_INT32 = 0,
            //!Integer 64 bit length
            TYPE_INT64,
            //!Double 64 bit length
            TYPE_DOUBLE,
            //!C string variable length
            TYPE_STRING,
            //!byte array variable length
            TYPE_BYTEARRAY,
            //!bool variable length
            TYPE_BOOLEAN,
            TYPE_CLUSTER,
            //!modifier to be ored to normal data types
            TYPE_ACCESS_ARRAY=0x100,
            TYPE_UNDEFINED
};
struct DatasetElement{
		std::string name;
		void* buffer;
		int size;
		dataTypes type;
  int internal;
  DatasetElement(){buffer=NULL;size=0;type=TYPE_UNDEFINED;internal=0;}
		void resize(int size);
		template<typename T>
		operator T() {
		        return *reinterpret_cast<T*>(buffer);
		    }
  ~DatasetElement(){DPRINT("deleting dataset element %s mem freed %d",name.c_str(),internal);if(internal && buffer)free(buffer);buffer=NULL;}
};

class DataSet {
public:
	typedef boost::shared_ptr<DatasetElement> DatasetElement_psh;
protected:

	std::vector<DatasetElement_psh> elems;
	std::map<std::string, DatasetElement_psh > elem_by_name;

	std::string name;
public:
	DataSet();

	DataSet(std::string name);
	virtual ~DataSet();
	void setDataSetName(const std::string &_name){name=_name;}
	void* add(const std::string& name,dataTypes type,std::string& pnt,int size=0){
				return add( name,type,(void*)pnt.c_str(), pnt.size());
	}
	template<typename T>
		void* add(const std::string& name,dataTypes type,T*pnt,int size=0){

			return add( name,type,(void*)pnt, size==0?sizeof(T):size);
		}
	template<typename T>
	void* add(const std::string& name,dataTypes type,T&pnt,int size=0){
		return add( name,type,(void*)&pnt, size==0?sizeof(T):size);
	}
	void* add(const std::string& name,dataTypes type,void*pnt,int size,int internal=0);

	void* add(const std::string& name,dataTypes type);
	void* add(const std::string& name,dataTypes type,int size);

	int set(int idx,void*ptr,int size);
	int set(const std::string& name,void*ptr,int size);
	const std::string & getName() const {return name;}
	const std::vector<DatasetElement_psh >& getDataSet() const {return elems;};
};
}
}
}
#endif /* DATA_CORE_DATASET_H_ */
