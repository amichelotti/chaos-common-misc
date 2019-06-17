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
	uint64_t quantum;
public:
	SchedTimeElem(const std::string& uid,int prio=0);
	virtual ~SchedTimeElem();
	void setQuantum(uint64_t usquanto );
	uint64_t getQuantum();
	bool hasToSched(uint64_t t);

};
}
} /* namespace misc */
} /* namespace common */

#endif /* SCHEDULER_SCHEDTIMEELEM_H_ */
