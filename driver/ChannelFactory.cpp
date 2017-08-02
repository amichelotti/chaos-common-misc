/*
 * ChannelFactory.cpp
 *
 *  Created on: Aug 2, 2017
 *      Author: michelo
 */

#include "ChannelFactory.h"
#include <common/serial/core/PosixSerialComm.h>
#include <common/debug/core/debug.h>
namespace common {
namespace misc {
namespace driver {

std::map<std::string,AbstractChannel_psh> ChannelFactory::unique_channels;


AbstractChannel_psh ChannelFactory::getChannel(std::string serial_dev,int baudrate,int parity,int bits,int stop,bool hwctrl){
	std::map<std::string,AbstractChannel_psh>::iterator i=unique_channels.find(serial_dev);
	if(i!=unique_channels.end()){
		DPRINT("retrieving SERIAL channel '%s' @%p",serial_dev.c_str(),i->second.get());
		return i->second;
	}
	common::serial::PosixSerialComm* ptr=new common::serial::PosixSerialComm(serial_dev,baudrate,parity,bits,stop,hwctrl);
	AbstractChannel_psh ret(ptr);
	unique_channels[serial_dev]=ret;
	DPRINT("creating SERIAL channel '%s' @%p",serial_dev.c_str(),ret.get());

	return ret;

}
AbstractChannel_psh ChannelFactory::getChannel(const std::string& ip_port ){
	AbstractChannel_psh p;
	DPRINT("creating TCP channel '%s' @%p",ip_port.c_str(),p.get());

	return p;
}
ChannelFactory::ChannelFactory() {
	// TODO Auto-generated constructor stub

}

ChannelFactory::~ChannelFactory() {
	// TODO Auto-generated destructor stub
}

} /* namespace driver */
} /* namespace misc */
} /* namespace common */
