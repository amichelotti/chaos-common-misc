/*
 * Worker.cpp
 *
 *  Created on: Mar 25, 2017
 *      Author: michelo
 */

#include "Worker.h"
#include <boost/chrono.hpp>
#include <algorithm>

namespace common {
namespace misc {
namespace scheduler {


template <class T>
void Worker<T>::stop() {
	if(run){
				run = 0;
				m_thread.join();
	}

}
template<class T>
Worker<T>::~Worker() {
	// TODO Auto-generated destructor stub
	stop();

}


} /* namespace scheduler */
} /* namespace misc */
} /* namespace common */
