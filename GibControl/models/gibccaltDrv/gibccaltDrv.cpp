/*
gibccaltDrv.cpp
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
//#define CHAOS
#ifdef CHAOS
#include <common/misc/driver/ConfigDriverMacro.h>
#include <chaos/common/data/CDataWrapper.h>
#endif
#include "gibccaltDrv.h"


using namespace std;
using namespace common::gibcontrol;
using namespace common::gibcontrol::models;
gibccaltDrv::gibccaltDrv(const std::string Parameters) {
}

/**********************************************************************/

int PingGib(const char* IP,int sec)
{
int    i, rc, on = 1;
   int    listen_sd;
   struct protoent *protocol;
   int    gnum;
   char   buffer[80];
   struct sockaddr_in   addr;
   struct timeval       tv;
   fd_set        fds;
   tv.tv_sec = sec;
   tv.tv_usec = 500000;
   /*************************************************************/
   /* Create an AF_INET stream socket to receive incoming       */
   /* connections on                                            */
   /*************************************************************/
   protocol=getprotobyname("tcp");
   listen_sd = socket(AF_INET, SOCK_STREAM,protocol->p_proto );
   if (listen_sd < 0)
   {
      perror("socket() failed");
      return -1;
   }

   /*************************************************************/
   /* Allow socket descriptor to be reuseable                   */
   /*************************************************************/
   rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
                   (char *)&on, sizeof(on));
   if (rc < 0)
   {
      perror("setsockopt() failed");
      close(listen_sd);
      return -1;
   }
   /*************************************************************/
   /* Set socket to be non-blocking.  All of the sockets for    */
   /* the incoming connections will also be non-blocking since  */

   /* they will inherit that state from the listening socket.   */
   /*************************************************************/
   rc = ioctl(listen_sd, FIONBIO, (char *)&on);
   if (rc < 0)
   {
      perror("ioctl() failed");
      close(listen_sd);
      return -1;
   }
   memset(&addr, 0, sizeof(addr));
   addr.sin_family      = AF_INET;
   addr.sin_addr.s_addr = inet_addr(IP);
   addr.sin_port        = htons(23);
   DPRINT("Pinging  to gib %s\n",IP);
     connect(listen_sd, (struct sockaddr *) &addr, sizeof(addr));

     FD_ZERO(&fds);
     FD_SET(listen_sd, &fds);
     rc = select(listen_sd+1, NULL, &fds, NULL, &tv);

     if((rc == 1) && (FD_ISSET(listen_sd, &fds)))
     {
        close(listen_sd);
        shutdown(listen_sd,2);
        return 1;
     } else
     {
        DPRINT(" ALEDEBUG  connect error\n");
        close(listen_sd);
        shutdown(listen_sd,2);
        return -1;
     }

}
/***************************************************************7*/



#ifdef CHAOS
gibccaltDrv::gibccaltDrv(const chaos::common::data::CDataWrapper &config) {
	
	DPRINT("received init parameter %s ",config.getJSONString().c_str());
	GET_PARAMETER_TREE((&config), driver_param)
	{
		GET_PARAMETER(driver_param, IP, string, 1);
		gibIP.assign(IP);
	}
	
	int isConnected=PingGib(this->gibIP.c_str(),3);
	

}
#endif
gibccaltDrv::~gibccaltDrv() {
}
int gibccaltDrv::setPulse(int32_t channel,int32_t amplitude,int32_t width,int32_t state) {
	return 0;
}
int gibccaltDrv::setChannelVoltage(int32_t channel,double Voltage) {
	return 0;
}
int gibccaltDrv::PowerOn(int32_t on_state) {
	int ret= PingGib(this->gibIP.c_str(),4);
	if (ret==1)
		return 0;
	else
	return -1;
}
int gibccaltDrv::getState(int32_t* state,std::string& desc) {
	return 0;
}
int gibccaltDrv::getVoltages(std::vector<double>& voltages) {
	return 0;
}
int gibccaltDrv::getNumOfChannels(int32_t* numOfChannels) {
	*numOfChannels=24;
	return 0;
}
int gibccaltDrv::getPulsingState(std::vector<int32_t>& amplitudes,std::vector<int32_t>& widthChannels) {
	return 0;
}
int gibccaltDrv::getSupplyVoltages(double* HVSupply,double* P5V,double* N5V) {
	return 0;
}
