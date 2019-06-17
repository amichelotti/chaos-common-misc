/*
 * SchedElem.cpp
 *
 *  Created on: Mar 25, 2017
 *      Author: michelo
 */

#include "SchedTimeElem.h"

namespace common {
namespace misc {
namespace scheduler {

SchedTimeElem::SchedTimeElem(const std::string& uid,int prio):SchedBasicElem(uid,prio) {
	// TODO Auto-generated constructor stub
	quantum=0;
}
bool SchedTimeElem::hasToSched(uint64_t t){
	dynPrio=(t-last_sched);

	if((t-last_sched)>=quantum){
		return true;
	}
	return false;
}
SchedTimeElem::~SchedTimeElem() {
	// TODO Auto-generated destructor stub
}

void SchedTimeElem::setQuantum(uint64_t quanto ){
	quantum= quanto;
}
uint64_t SchedTimeElem::getQuantum(){
	return quantum;
}
} /* namespace misc */
} /* namespace common */
}
