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
	boost::mutex::scoped_lock(chanmutex);
	std::map<std::string,AbstractChannel_psh>::iterator i=unique_channels.find(serial_dev);
	if(i!=unique_channels.end()){
		DPRINT("retrieving SERIAL channel '%s' @%p in use count %ld",serial_dev.c_str(),i->second.get(),i->second.use_count());
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
	boost::mutex::scoped_lock(chanmutex);

	DPRINT("creating TCP channel '%s' @%p",ip_port.c_str(),p.get());

	return p;
}

void ChannelFactory::removeChannel(const std::string& uid){
	boost::mutex::scoped_lock(chanmutex);

	std::map<std::string,AbstractChannel_psh>::iterator i=unique_channels.find(uid);
	if(i!=unique_channels.end()){
		if(i->second.use_count()==1){
			DPRINT("REMOVING CHANNEL '%s' @%p in use %ld",uid.c_str(),i->second.get(),i->second.use_count());
			unique_channels.erase(i);
		} else {
			DPRINT("CANNOT REMOVE CHANNEL '%s' @%p in use %ld",uid.c_str(),i->second.get(),i->second.use_count());
		}
	}
}
void ChannelFactory::removeChannel(AbstractChannel_psh& ch){
	std::string uid=ch->getUid();

	DPRINT("ATTEMPT TO REMOVE CHANNEL'%s' @%p in use in %ld places",uid.c_str(),ch.get(),ch.use_count());
	ch.reset();

	removeChannel(uid);
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
