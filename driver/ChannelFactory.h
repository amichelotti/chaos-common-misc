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
	static void removeChannel(const std::string& uid);
	static void removeChannel(AbstractChannel_psh& ch);

};

} /* namespace drover */
} /* namespace misc */
} /* namespace common */

#endif /* DRIVER_CHANNELFACTORY_H_ */
