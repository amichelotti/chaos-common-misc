/*
 * ChannelFactory.h
 *
 *  Created on: Aug 2, 2017
 *      Author: michelo
 */

#ifndef DRIVER_CHANNELFACTORY_H_
#define DRIVER_CHANNELFACTORY_H_
#include "AbstractChannel.h"
#include <map>
namespace common {
namespace misc {
namespace driver {

class ChannelFactory {
	static std::map<std::string,AbstractChannel_psh> unique_channels;
public:
	ChannelFactory();
	virtual ~ChannelFactory();
	// retrieve a Serial Channel
	static AbstractChannel_psh getChannel(std::string serial_dev,int baudrate,int parity,int bits,int stop,bool hwctrl=false);
	//retrieve a tcp channel
	static AbstractChannel_psh getChannel(const std::string& ip_port );
};

} /* namespace drover */
} /* namespace misc */
} /* namespace common */

#endif /* DRIVER_CHANNELFACTORY_H_ */
