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

namespace common  {
namespace misc {
namespace data {
class DataSet {

protected:
	struct dataset_ele{
		std::string name;
		void* buffer;
		int size;
		int type;
	};

	std::vector<dataset_ele*> elems;
	std::map<std::string, dataset_ele*> elem_by_name;

	std::string name;
public:
	DataSet();

	DataSet(std::string name);
	virtual ~DataSet();
	void setDataSetName(const std::string &_name){name=_name;}
	int add(const std::string& name,int type);
	int set(int idx,void*ptr,int size);
	int set(const std::string& name,void*ptr,int size);
	std::string & getName(){return name;}

};
}
}
}
#endif /* DATA_CORE_DATASET_H_ */
