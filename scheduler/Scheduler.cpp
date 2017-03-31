/*
 * Scheduler.cpp
 *
 *  Created on: Mar 25, 2017
 *      Author: michelo
 */

#include "Scheduler.h"
#include <common/debug/core/debug.h>
#include <boost/chrono.hpp>
#include <algorithm>

namespace common {
namespace misc {
namespace scheduler {


void Scheduler::sched_task(){
	DPRINT("Scheduler Started");
	std::vector<SchedBasicElem*>::iterator i;
	uint64_t begin_time,accum=0;
	while(run){
		begin_time=common::debug::getUsTime();
		std::sort(v_sched_elem.begin(),v_sched_elem.end(),schedElemCompare);

		for(i=v_sched_elem.begin();i!=v_sched_elem.end();i++){
			DPRINT("shedule elem \"%s\" prio:%d",(*i)->getUid().c_str(),(*i)->dynPrio);
			(*i)->sched();
		}
		if(npoints%avg_points){
			accum+=(common::debug::getUsTime()-begin_time);
			schedule_avg= (1.0*accum)/npoints;
		} else {
			accum=0;
		}
	}

}
void Scheduler::add(const std::string& uid,SchedBasicElem* el){
	boost::mutex::scoped_lock a;
	if(v_elem_map.find(uid)==v_elem_map.end()){
		v_sched_elem.push_back(el);
	}
	v_elem_map[uid]=el;
}

void Scheduler::remove(const std::string& uid){
	boost::mutex::scoped_lock a;
	element_map_t::iterator i=v_elem_map.find(uid);
	if(i!=v_elem_map.end()){
		for(std::vector<SchedBasicElem*>::iterator ii=v_sched_elem.begin();ii!=v_sched_elem.begin();ii++){
			if(*ii == v_elem_map[uid]){
				v_sched_elem.erase(ii);
				break;
			}
		}
		v_elem_map.erase(i);
	}

}
void Scheduler::stop(){
	if(run){
			run = 0;
			m_thread.join();
	}
}
void Scheduler::start(){
	if(run==0){
		run =1;
		m_thread=boost::thread(&Scheduler::sched_task,this);
	}

}

Scheduler::Scheduler():avg_points(AVG_POINTS) {
	run=0;
	npoints=0;
	// TODO Auto-generated constructor stub

}

Scheduler::~Scheduler() {
	// TODO Auto-generated destructor stub
	stop();

}

} /* namespace scheduler */
} /* namespace misc */
} /* namespace common */
