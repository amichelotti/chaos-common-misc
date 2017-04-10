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

class Worker {

	static int id;
	int myid;
protected:
	boost::thread m_thread;


	int run;
	uint32_t npoints;
	double schedule_avg;
	const int avg_points;
	void sched_task();
public:
	void stop();
	void start();
	virtual int process()=0;
	int getWorkers() const {return id;}
	int getWorkerID() const {return myid;}

	Worker();
	virtual ~Worker();
};

} /* namespace scheduler */
} /* namespace misc */
} /* namespace common */

#endif /* SCHEDULER_SCHEDULER_H_ */
