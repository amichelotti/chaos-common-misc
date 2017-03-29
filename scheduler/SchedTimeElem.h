/*
 * SchedElem.h
 *
 *  Created on: Mar 25, 2017
 *      Author: michelo
 */

#ifndef SCHEDULER_SCHEDTIMEELEM_H_
#define SCHEDULER_SCHEDTIMEELEM_H_
#include <stdint.h>
#include "SchedBasicElem.h"

namespace common {
namespace misc {
namespace scheduler {

class SchedTimeElem :public SchedBasicElem{
	uint64_t lastSched;
	uint64_t quantum;
public:
	SchedTimeElem(const std::string& uid,int prio=0);
	virtual ~SchedTimeElem();
	void setQuantum(uint64_t quanto );
	uint64_t getQuantum();

};
}
} /* namespace misc */
} /* namespace common */

#endif /* SCHEDULER_SCHEDTIMEELEM_H_ */
