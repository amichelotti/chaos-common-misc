/*
 * Scheduler.h
 *
 *  Created on: Mar 25, 2017
 *      Author: michelo
 */

#ifndef SCHEDULER_SCHEDULER_H_
#define SCHEDULER_SCHEDULER_H_
#include "SchedBasicElem.h"
#include <boost/thread.hpp>
#include <vector>
#include <boost/thread/mutex.hpp>

namespace common {
namespace misc {
namespace scheduler {
#define AVG_POINTS 100
typedef std::map<std::string,SchedBasicElem*> element_map_t;
class Scheduler {

	element_map_t v_elem_map;
	std::vector<SchedBasicElem*> v_sched_elem;
	boost::thread m_thread;
	boost::mutex m_mutex;
	int run;
	void sched_task();
	double schedule_avg;
	const int avg_points;
	uint32_t npoints;
public:
	void add(const std::string& uid,SchedBasicElem*);
    /**
      return >0 on success
    */
    int remove(const std::string& uid);
	void stop();
	void start();

	Scheduler();
	virtual ~Scheduler();
};
} /* namespace scheduler */
} /* namespace misc */
} /* namespace common */

#endif /* SCHEDULER_SCHEDULER_H_ */
