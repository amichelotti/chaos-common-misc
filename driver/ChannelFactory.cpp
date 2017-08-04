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
AbstractChannel_psh ChannelFactory::getChannel(const CDataWrapper& json )  throw(chaos::CException) {
	if(json.hasKey("channel")){
		CDataWrapper* chanparam=json.getCSDataValue("channel");
		if(chanparam){
			if(chanparam->hasKey("dev")){
				// serial channel
				std::string dev= chanparam->getStringValue("dev");
				int baudrate=9800;
				int stop=0;
				int parity=0;
				int bits=8;
				int hwctrl=0;
				if(chanparam->hasKey("baudrate")){
					baudrate=chanparam->getInt32Value("baudrate");
				} else {
					throw chaos::CException(-11, "'baudrate' 9600-115200 missing",__PRETTY_FUNCTION__);;
				}
				if(chanparam->hasKey("parity")){
					parity=chanparam->getInt32Value("parity");
				} else {
					throw chaos::CException(-11, "'parity' 0-1 missing",__PRETTY_FUNCTION__);
				}
				if(chanparam->hasKey("stop")){
					stop=chanparam->getInt32Value("stop");
				} else {
					throw chaos::CException(-11, "'stop' 7-8 bit missing",__PRETTY_FUNCTION__);
				}
				if(chanparam->hasKey("hwctrl")){
					hwctrl=chanparam->getInt32Value("hwctrl");
				} else {
					throw chaos::CException(-11, "'hwctrl' bit missing",__PRETTY_FUNCTION__);
				}
				return getChannel(dev,baudrate,parity,bits,stop,hwctrl);
			} else if(chanparam->hasKey("tcp")){
				std::string dev= chanparam->getStringValue("tcp");
				int port=80;
				if(chanparam->hasKey("port")){
					port=chanparam->getInt32Value("port");
				} else {
					throw chaos::CException(-11, "'port' 0-65536 missing",__PRETTY_FUNCTION__);
				}
				std::stringstream ss;
				ss<<dev<<":"<<port;
				return getChannel(ss.str());

				// tcp channel
			} else {
				throw chaos::CException(-11, "'serial' or 'tcp' keys and parameters must be specified",__PRETTY_FUNCTION__);;

			}
		}


	} else {
		throw chaos::CException(-11, "channel key is required",__PRETTY_FUNCTION__);;

	}
	throw chaos::CException(-11, "not a correct channel specification",__PRETTY_FUNCTION__);;

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
		DPRINT("CHANNEL REMOVED in use %d",ch.use_count());
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
