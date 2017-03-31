/*
 * Worker.h
 *
 *  Created on: Mar 25, 2017
 *      Author: michelo
 */

#ifndef _WORKER_H_
#define _WORKER_H_

#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/thread/mutex.hpp>
#include <common/debug/core/debug.h>
namespace common {
namespace misc {
namespace scheduler {

template<class DATA>
class Worker {

	boost::lockfree::queue<DATA>& mqueue;
protected:
	boost::thread m_thread;


	int run;
	uint32_t npoints;
	double schedule_avg;
	const int avg_points;
	void sched_task(){
		DPRINT("Worker Started");

		uint64_t begin_time,accum=0;
		DATA obj;
		while(run){

			begin_time=common::debug::getUsTime();
			if(mqueue.empty()){
				usleep(10000);
				continue;
			}
			if(mqueue.pop(obj)){
				process(obj);
			}
			if(npoints%avg_points){
						accum+=(common::debug::getUsTime()-begin_time);
						schedule_avg= (1.0*accum)/npoints;
					} else {
						accum=0;
					}
		}
	}
public:
	void stop();
	void start(){
		if(run==0){
			run =1;
			m_thread=boost::thread(&sched_task,this);
		}
	}
	int process(DATA&)=0;
	Worker(boost::lockfree::queue<DATA>& d):mqueue(d),avg_points(100){
		run=0;
		npoints=0;
		schedule_avg=0;
	}
	virtual ~Worker();
};
} /* namespace scheduler */
} /* namespace misc */
} /* namespace common */

#endif /* SCHEDULER_SCHEDULER_H_ */
