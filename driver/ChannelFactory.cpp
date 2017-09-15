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
#ifdef CHAOS
using namespace chaos::common::data;
AbstractChannel_psh ChannelFactory::getChannel(const chaos::common::data::CDataWrapper& json )  throw(chaos::CException) {
	GET_PARAMETER_TREE((&json),channel){
		GET_PARAMETER_DO(channel,serdev,string,0){
			//serial channel
			GET_PARAMETER(channel,baudrate,int32_t,1);
			GET_PARAMETER(channel,parity,int32_t,1);
			GET_PARAMETER(channel,stop,int32_t,1);
			GET_PARAMETER(channel,hwctrl,int32_t,1);
			GET_PARAMETER(channel,bits,int32_t,1);
			return getChannel(serdev,baudrate,parity,bits,stop,hwctrl);

		}
		GET_PARAMETER_DO(channel,tcp,string,0){
			GET_PARAMETER(channel,port,int32_t,1);
			std::stringstream ss;
			ss<<tcp<<":"<<port;
			return getChannel(ss.str());

		}
	}
}
#endif

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
	if(ch.get()==0){
		DPRINT("CHANNEL REMOVED in use %ld",ch.use_count());
		return;
	}
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
