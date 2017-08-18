/*
 * SchedBasicElem.h
 *
 *  Created on: Mar 25, 2017
 *      Author: michelo
 */

#ifndef SCHEDULER_SCHEDBASICELEM_H_
#define SCHEDULER_SCHEDBASICELEM_H_
#include <string>
#include <stdint.h>
namespace common {
namespace misc {
namespace scheduler {

class SchedBasicElem {
	std::string uid;
public:
	uint64_t last_sched;

	int basePrio; // fixed by user/app
	int dynPrio;
	SchedBasicElem(const std::string& uid,int prio=0);
	virtual ~SchedBasicElem();

	/* should return the desired schedule time */
	virtual uint64_t sched(uint64_t sched_time)=0;
	/*determine if to schedule */

	virtual bool hasToSched(uint64_t sched_time)=0;

	void setPrio(int prio){basePrio=prio;}
	int getPrio(){return basePrio;}
	std::string getUid(){return uid;}
	void setUid(const std::string& id ){uid=id;}
	bool operator()(int prioa,int priob){return prioa>priob;}

};
	bool schedElemCompare(const SchedBasicElem*a,const SchedBasicElem*b);
} /* namespace scheduler */
} /* namespace misc */
} /* namespace common */

#endif /* SCHEDULER_SCHEDBASICELEM_H_ */
