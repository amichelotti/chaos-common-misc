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
#include <boost/lexical_cast.hpp>

namespace common {
namespace misc {
namespace scheduler {


void Scheduler::sched_task(){
	DPRINT("Scheduler Started");
	std::vector<SchedBasicElem*>::iterator i;
	uint64_t begin_time,accum=0;
	while(run){
		uint64_t waitmin=1000*1000;

		begin_time=common::debug::getUsTime();
		{
		ReadLock a(m_mutex);
		std::sort(v_sched_elem.begin(),v_sched_elem.end(),schedElemCompare);
		
		for(i=v_sched_elem.begin();i!=v_sched_elem.end();i++){
			uint64_t st=common::debug::getUsTime();
			if((*i)->hasToSched(st)){
			//	DPRINT("schedule elem \"%s\" prio:%d",(*i)->getUid().c_str(),(*i)->dynPrio);
				waitmin=std::min(waitmin,(*i)->sched(st));
				(*i)->last_sched=st;
			}

		}
		}
		if(++npoints%avg_points){
			uint64_t res=(common::debug::getUsTime()-begin_time);
			accum+=res;
			schedule_avg= (1.0*accum)/npoints;
		//	DPRINT("tot sched time %f ms, avg %f ms points %d",1.0*res/1000.0, schedule_avg/1000.0,npoints)
		} else{
			std::string th=boost::lexical_cast<std::string>(m_thread.get_id());
			DPRINT("[%s] scheduling %ld elems average %f ms",th.c_str(),v_sched_elem.size(),schedule_avg/1000.0);
			accum=0;
			npoints=0;
		}
		if(waitmin){

			//DPRINT("waiting for %lld us",waitmin);

			usleep(waitmin);
		}
	}

}
void Scheduler::add(const std::string& uid,SchedBasicElem* el){
	WriteLock a(m_mutex);
	if(v_elem_map.find(uid)==v_elem_map.end()){
		v_sched_elem.push_back(el);
	}
	v_elem_map[uid]=el;
}

int  Scheduler::remove(const std::string& uid){
	WriteLock a(m_mutex);
	element_map_t::iterator i=v_elem_map.find(uid);
	if(i!=v_elem_map.end()){
        for(std::vector<SchedBasicElem*>::iterator ii=v_sched_elem.begin();ii!=v_sched_elem.end();ii++){
            if(*ii == i->second){
				v_sched_elem.erase(ii);
				break;
			}
		}
		v_elem_map.erase(i);
        return 1;
	}
    return 0;
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
