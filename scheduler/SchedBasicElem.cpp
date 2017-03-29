/*
 * SchedBasicElem.cpp
 *
 *  Created on: Mar 25, 2017
 *      Author: michelo
 */

#include "SchedBasicElem.h"

namespace common {
namespace misc {
namespace scheduler {

SchedBasicElem::SchedBasicElem(const std::string& _uid,int prio):uid(_uid),basePrio(prio),dynPrio(prio) {
	// TODO Auto-generated constructor stub
}

SchedBasicElem::~SchedBasicElem() {
	// TODO Auto-generated destructor stub
}
bool schedElemCompare(const SchedBasicElem*a,const SchedBasicElem*b){
	return (a->dynPrio) > (b->dynPrio);
}
} /* namespace scheduler */
} /* namespace misc */
} /* namespace common */

