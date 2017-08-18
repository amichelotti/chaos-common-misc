/*
 * Worker.cpp
 *
 *  Created on: Mar 25, 2017
 *      Author: michelo
 */

#include <boost/chrono.hpp>
#include <algorithm>
#include "Worker.h"

namespace common {
namespace misc {
namespace scheduler {
int Worker::id =0;

void Worker::sched_task(){
	DPRINT("Worker Started %d",myid);

	uint64_t begin_time,accum=0;
	while(run){

		begin_time=common::debug::getUsTime();
		process();
		if(npoints%avg_points){
					accum+=(common::debug::getUsTime()-begin_time);
					schedule_avg= (1.0*accum)/npoints;
				} else {
					accum=0;
				}
	}
}
void  Worker::stop() {
	if(run){
				run = 0;
				m_thread.join();
	}

}
void  Worker::start(){
	if(run==0){
		run =1;
		m_thread=boost::thread(&Worker::sched_task,this);
	}
}
Worker::Worker():avg_points(100){
		run=0;
		npoints=0;
		schedule_avg=0;

		myid=id++;
	}
Worker::~Worker(){
		stop();
		id--;

	}
} /* namespace scheduler */
} /* namespace misc */
} /* namespace common */
