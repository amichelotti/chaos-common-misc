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


/**********************************************************************/

int PingGib(const char* IP,int sec) {
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

char* getCommandToRead(int baseAddress,unsigned int offset)
{

  int i;
  int somma;
  char* toRet,val[8];
  toRet=(char*)malloc(sizeof(char)*12);
  toRet[0]='r';
  bzero(val,8);
  somma=baseAddress+offset;
  sprintf(val,"%x\0",somma);
  for (i=0; i< strlen(val);i++)
  {
    toRet[i+1]=val[i];
  }
  //fprintf(stderr,"val is %s\n",val);
  toRet[++i]='\r';
  toRet[++i]='\n';
  toRet[++i]='\0';
//  fprintf(stderr,"toRet is %s\n",toRet);
return toRet;
}

const char* getCommandToWrite(int baseAddress,int chan,float value)
{
  int i,address,valint,zeroToAdd;
  char* toRet,val[8];
  char *tmp;
  std::string  cppString="w";
  if (value > 75.0f)
  {
     printf("Error voltage too high( > 75V))\n");
     return strdup("");
  }

  toRet=(char*)malloc(sizeof(char)*32);
  toRet[0]='w';
  if (chan < 8) address=baseAddress + chan*4;
  if ( (chan >7) && (chan < 16) ) address=baseAddress +0x100+((chan-8)*4);
  if (chan>15)  address= baseAddress + 0x200 + ((chan-16)*4) ;
  sprintf(val,"%x\0",address);
  cppString+=val;
  for (i=0; i< strlen(val);i++)
  {
    toRet[i+1]=val[i];
  }
  //value deve essere una stringa di 2,4,8 caratteri
  //valint=(int)( (value -  63.75 )*2604.16);
  valint=(int)( (value -  64 )*2604.16);
  sprintf(val,"%x\0",valint);
  zeroToAdd=8 - strlen(val);
  
  for (i=zeroToAdd; i > 0; i-- )
  {
     cppString+="0";
    /*tmp=toRet;
    toRet=appiccica_stringhe(2,toRet,"0");
    free(tmp);*/
  }
  tmp=toRet;
  cppString+=val;
  //toRet=appiccica_stringhe(2,toRet,val);
  //free(tmp);
  tmp=toRet;
  cppString+="\r\n\0";
  //toRet=appiccica_stringhe(2,toRet,"\r\n\0");
  //free(tmp);
  printf ("zeroToAdd is %d \n",zeroToAdd);
  printf("cppString is %s  adding Value %x\n",cppString.c_str(),valint);
  return cppString.c_str();
}

/*******************************************************************/
int decodeReadString(char* buf)
{
  int i,retVal;
  char tmp[32];
  for (i=9; i < strlen(buf);i++)
  {
    if ( !isalnum(buf[i]) )
    {
        tmp[i-9]='\0';
        break;
    }
    tmp[i-9]=buf[i];
  }
  i=sscanf(tmp,"%x",&retVal);
  if (i != 1)
    retVal=-1;
  return retVal;
}
/********************************************************************/




int OpenSocket(std::string IP)
{
   int rc,mysocket;
   unsigned int dst;
   struct sockaddr_in server;
   struct protoent *protocol;
   protocol=getprotobyname("tcp");
   mysocket=socket(AF_INET,SOCK_STREAM,protocol->p_proto);
   inet_pton(AF_INET,IP.c_str(),&dst);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr=(unsigned int)dst;
    server.sin_port=htons(23);
    DPRINT("Connecting  to gib %s\n",IP.c_str());
    rc=connect(mysocket,(struct sockaddr*)&server,sizeof(server) );
    if (rc < 0)
    {
      printf("Error connecting\n");
      close(mysocket);
      shutdown(mysocket,2);
      return -1;
    }
    else
    {
      printf("Connected to gib %s\n",IP.c_str());
      return mysocket;
    }




}

gibccaltDrv::gibccaltDrv(const std::string Parameters) {
      this->numOfChannels=24;
}
#ifdef CHAOS
gibccaltDrv::gibccaltDrv(const chaos::common::data::CDataWrapper &config) {
	
	DPRINT("received init parameter %s ",config.getJSONString().c_str());
	GET_PARAMETER_TREE((&config), driver_param)
	{
		GET_PARAMETER(driver_param, IP, string, 1);
		gibIP.assign(IP);
	}
	this->numOfChannels=24;
	int isConnected=PingGib(this->gibIP.c_str(),3);
      if (!isConnected)
      {
            DPRINT("WARNING gib seems unreachable");
      }
	

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
    
    int mysocket,rc;
    mysocket=OpenSocket(this->gibIP);
    char* comando=getCommandToRead(0xCEA00004,0);
    char buffer[256];
    DPRINT("ALEDEBUG comando is ",comando);
    rc=write(mysocket,comando,11);
    free(comando);
    bzero(buffer,256);
    rc=read(mysocket,buffer,19);
    int auxVar=decodeReadString(buffer);
    DPRINT("pulsing state received %d",auxVar);
    close(mysocket);
    shutdown(mysocket,2);
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
