/*
 * ChannelFactory.h
 *
 *  Created on: Aug 2, 2017
 *      Author: michelo
 */

#ifndef DRIVER_CHANNELFACTORY_H_
#define DRIVER_CHANNELFACTORY_H_
#include "AbstractChannel.h"
#include <boost/thread.hpp>
#include <common/misc/driver/ConfigDriverMacro.h>

#ifdef CHAOS
#include <chaos/common/exception/CException.h>
#include <chaos/common/data/CDataWrapper.h>

#endif
#include <map>
namespace common {
namespace misc {
namespace driver {

class ChannelFactory {
	static std::map<std::string,AbstractChannel_psh> unique_channels;
	boost::mutex chanmutex;
public:
	ChannelFactory();
	virtual ~ChannelFactory();
	// retrieve a Serial Channel
	static AbstractChannel_psh getChannel(std::string serial_dev,int baudrate,int parity,int bits,int stop,bool hwctrl=false);
	//retrieve a tcp channel
	static AbstractChannel_psh getChannel(const std::string& ip_port );
#ifdef CHAOS
	static AbstractChannel_psh getChannel(const chaos::common::data::CDataWrapper& config)  throw (chaos::CException);
#endif
	static void removeChannel(const std::string& uid);
	static void removeChannel(AbstractChannel_psh& ch);

};

} /* namespace drover */
} /* namespace misc */
} /* namespace common */

#endif /* DRIVER_CHANNELFACTORY_H_ */
