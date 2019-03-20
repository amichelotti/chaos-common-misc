/*
HetPicTCPDriver.cpp
!CHAOS
Created by CUGenerator

Copyright 2013 INFN, National Institute of Nuclear Physics
Licensed under the Apache License, Version 2.0 (the "License")
you may not use this file except in compliance with the License.
      You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <common/debug/core/debug.h>
//#define CHAOS;
#ifdef CHAOS
#include <common/misc/driver/ConfigDriverMacro.h>
#include <chaos/common/data/CDataWrapper.h>
#endif
#include "HetPicTCPDriver.h"

using namespace common::hetpic;
using namespace common::hetpic::models;
using namespace std;

HetPicTCPDriver::HetPicTCPDriver(const std::string Parameters) {
}
#ifdef CHAOS
HetPicTCPDriver::HetPicTCPDriver(const chaos::common::data::CDataWrapper &config) { 
	GET_PARAMETER_TREE((&config), driver_param)
	{
		GET_PARAMETER(driver_param, IP, string, 1);
		IPaddress.assign(IP);
	}
	this->SetUp();
}
#endif
HetPicTCPDriver::~HetPicTCPDriver() {
}
void HetPicTCPDriver::SetUp()
{
	unsigned int dst;
	this->numOfChannels=32;
	this->highThr.resize(this->numOfChannels);
	this->lowThr.resize(this->numOfChannels);
	struct protoent *protocol;
	protocol=getprotobyname("tcp");
	mysocket=socket(AF_INET,SOCK_STREAM,protocol->p_proto);
	inet_pton(AF_INET,this->IPaddress.c_str(),&dst);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=(unsigned int) dst;
	server.sin_port=htons(23);
}


int HetPicTCPDriver::SetHighThreshold(int32_t channel,int32_t millivolts) {
	return 0;
}
int HetPicTCPDriver::SetLowThreshold(int32_t channel,int32_t millivolts) {
	return 0;
}
int HetPicTCPDriver::setPulse(int32_t value) {
	return 0;
}
int HetPicTCPDriver::getNumberOfChannel(int32_t& chanNum) {
	chanNum=this->numOfChannels;
	return 0;
}
int HetPicTCPDriver::getStatus(int32_t& status) {
	status=77;
	return 0;
}
int HetPicTCPDriver::getHighThresholds(std::vector<int32_t>& highThresholds) {
  	int rc;
	  DPRINT("ALEDEBUG TRYING TO ASK H THR");
	rc=connect(this->mysocket,(struct sockaddr*) &server,sizeof(server) );
	if (rc==0)
	{
		std::string retStr="";
		char buffer[2048];
		DPRINT("Reading PIC");
		rc=write(this->mysocket,"h\r\n",3);
		rc=read(this->mysocket,buffer,2048);
		retStr+=buffer;
		rc=read(this->mysocket,buffer,2048);
		retStr+=buffer;
		rc=read(this->mysocket,buffer,2048);
		retStr+=buffer;
		rc=read(this->mysocket,buffer,2048);
		retStr+=buffer;
		DPRINT("Pic sent this %s",retStr.c_str());

		
		shutdown(this->mysocket,2);
		highThresholds=this->highThr;

	}
	else 
	{
		DPRINT("ALEDEBUG FAILED CONNECTION (%d)",rc);
		return rc;
	}


	return 0;
}
int HetPicTCPDriver::getLowThresholds(std::vector<int32_t>& lowThresholds) {
		int rc;
	  DPRINT("ALEDEBUG TRYING TO ASK L THR");
	rc=connect(this->mysocket,(struct sockaddr*) &server,sizeof(server) );
	DPRINT("ALEDEBUG connect ended");
	if (rc==0)
	{
		std::string retStr="";
		char buffer[2048];
		DPRINT("Reading PIC");
		rc=write(this->mysocket,"l\r\n",3);
		rc=read(this->mysocket,buffer,2048);
		retStr+=buffer;
		//rc=read(this->mysocket,buffer,2048);
		//retStr+=buffer;
		//rc=read(this->mysocket,buffer,2048);
		//retStr+=buffer;
		//rc=read(this->mysocket,buffer,2048);
		//retStr+=buffer;
		DPRINT("Pic sent this %s",retStr.c_str());

		
		shutdown(this->mysocket,2);
		
		lowThresholds=this->lowThr;
	}
	else
	{
		DPRINT("ALEDEBUG FAILED CONNECTION (%d)",rc);
		return rc;
	}
	
	return 0;
}
