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
   //DPRINT("Pinging  to gib %s\n",IP);
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
        DPRINT(" PingGib Connect error\n");
        close(listen_sd);
        shutdown(listen_sd,2);
        return -1;
     }

}
/***************************************************************7*/

std::string getCommandToRead(int baseAddress,unsigned int offset)
{
  int i;
  int somma;
  char val[9];
  std::string cppString="r";
  bzero(val,9);
  somma=baseAddress+offset;
  //DPRINT("ALEDEBUG 01 somma: %x",somma);
  sprintf(val,"%x",somma);
  cppString+=val;
  cppString+="\r\n";
  //DPRINT("ALEDEBUG created command  %s",cppString.c_str());
  //DPRINT("Lunghezza comando %d",strlen(cppString.c_str()));
  return cppString;
}

std::string getWriteCommand(int baseAddress, int offset, unsigned int value)
{
  int i,address,valint,zeroToAdd;
  char val[9];
  std::string  cppString="w";
  address=baseAddress + offset;
  sprintf(val,"%x",address);
  cppString+=val;
  sprintf(val,"%x",value);
  zeroToAdd=8 - strlen(val);
  for (i=zeroToAdd; i > 0; i-- )
  {
    cppString+="0";
  }
  cppString+=val;
  cppString+="\r\n\0";
  //DPRINT("ALEDEBUG created  write command  %s",cppString.c_str());
  //DPRINT("Lunghezza comando %d",strlen(cppString.c_str()));
  return cppString;

}




/*const char* getCommandToWrite(int baseAddress,int chan,float value)
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
}*/

/*******************************************************************/
int decodeReadString(char* buf)
{
  int i,retVal=-1;
  //char tmp[32];
  std::string retStr="";
  for (i=9; i < strlen(buf);i++)
  {
    if ( !isalnum(buf[i]) )
    {
        break;
    }
    retStr+=buf[i];
    //tmp[i-9]=buf[i];
  }
  i=sscanf(retStr.c_str(),"%x",&retVal);
  if (i != 1)
    retVal=-99;
  return retVal;
}

int convertPulseAmplitude(int Ampl) {
   return (int) ((Ampl/304)*1000000);
}

int convertPulseWidth(int Width) {
   return (int) (Width*1000/5.3);
}



unsigned int getDACCode(double Val)
 {  return (unsigned int) (((Val - 23.72)* 1000000 )/ 324); }

double getADCValue(int counts){
   return ((double) 24 + 0.000367 * counts);


 }
double ConvertHV(int bits) {
   return  2.585*bits*0.001; }
double ConvertLV(int bits) {
   return 0.000418*bits;
}


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
      return GIB_UNREACHABLE;
    }
    else
    {
      printf("Connected to gib %s\n",IP.c_str());
      char buffer[256];
      //rc=read(mysocket,buffer,256); //svuotamento buffer
      return mysocket;
    }




}

int gibccaltDrv::ReadGib(){
   int mysocket,rc;
   time_t now=time(NULL);
   if ((now-this->lastUpdate) <  2)
      return 2;
    if (PingGib(this->gibIP.c_str(),1)!= 1)
    {
      return GIB_UNREACHABLE;

    }
    this->lastUpdate=now;
    mysocket=OpenSocket(this->gibIP);
     if (mysocket < 0)
     {
            return GIB_UNREACHABLE;
      }
      else
     {
        char buffer[256];
        rc=read(mysocket,buffer,256);//SVUOTAMENTO BUFFER
        std::string comando=getCommandToRead(0xCEA00004,0);
        rc=write(mysocket,comando.c_str(),11);
        bzero(buffer,256);
        rc=read(mysocket,buffer,19);
        int auxVar=decodeReadString(buffer);
        DPRINT("buffer received %s pulsing state received %x",buffer,auxVar);
        this->pulsingstate=auxVar;
        comando=getCommandToRead(0xCEA00000,0);
        rc=write(mysocket,comando.c_str(),comando.length());
        bzero(buffer,256);
        rc=read(mysocket,buffer,19);
        auxVar=decodeReadString(buffer);
        this->internalState=((auxVar & 0x100) != 0);
        
        //Terzo ADC
        for (int i=0; i < 7; i++)
              {
                if ( (i==2) || (i == 3) )
                      continue;
                comando=getCommandToRead(0x83D00700,i*4);
                rc=write(mysocket,comando.c_str(),11);
                bzero(buffer,256);
                rc=read(mysocket,buffer,19);
                //DPRINT("sent %s Received buffer %s rc is %d",comando.c_str() ,buffer, rc);
                if (i<2)
                {
                      this->Temperature[i]=decodeReadString(buffer);
                      //DPRINT("Temperature is %f",this->Temperature[i]);
                }
                      
                if (i == 4)
                      this->SupplyHV=ConvertHV(decodeReadString(buffer));
                if ((i == 5) || (i==6) )
                {
                  if (i==5)
                    this->Pos5=ConvertLV(decodeReadString(buffer));
                    else
                    this->Neg5=ConvertLV(decodeReadString(buffer));
                }
              }
       for (int i=0;i < this->numOfChannels;++i)
       {
        if (i < 8)
          comando=getCommandToRead(0x83D00400,i*4);
        if ((i>=8) && (i < 16))
          comando=getCommandToRead(0x83D00500,(i-8)*4);
        if (i>=16)
          comando=getCommandToRead(0x83D00600,(i-16)*4);
        rc=write(mysocket,comando.c_str(),11);
        bzero(buffer,256);
        rc=read(mysocket,buffer,19);
        auxVar=decodeReadString(buffer);
        this->ChannelVoltages[VettoreMapSipm[i]]=getADCValue(auxVar);
       }
        SetInternalState();
        close(mysocket);
        shutdown(mysocket,2);
      }
      return 0;

 }

void gibccaltDrv::SetInternalState() {
  if ((this->pulsingstate==0) || (this->pulsingstate==0xFFFFF) || (this->pulsingstate==0xffffffff))
  {
    DOWNMASK(this->internalState,GIBCONTROL_PULSING);
  }
  else
  {
    UPMASK(this->internalState,GIBCONTROL_PULSING);
  }

}

std::string gibccaltDrv::DecodeStatus() {
  std::string retString="";
  if (CHECKMASK(this->internalState,GIBCONTROL_SUPPLIED))
  {
    retString+="Power On ";
  }
  else
  {
    retString+="Power Off ";
  }
  if (CHECKMASK(this->internalState,GIBCONTROL_PULSING))
  {
    retString+=" Pulsing";
  }
  return retString;
}

void gibccaltDrv::SetUp() {
     this->numOfChannels=24;
      this->ChannelVoltages.resize(this->numOfChannels);
      this->ChannelAmplitudes.resize(this->numOfChannels);
      this->ChannelWidths.resize(this->numOfChannels);
      this->lastUpdate=0;
}

gibccaltDrv::gibccaltDrv(const std::string Parameters) {
     this->SetUp();
}
#ifdef CHAOS
gibccaltDrv::gibccaltDrv(const chaos::common::data::CDataWrapper &config) {

	DPRINT("received init parameter %s ",config.getJSONString().c_str());
	GET_PARAMETER_TREE((&config), driver_param)
	{
		GET_PARAMETER(driver_param, IP, string, 1);
		gibIP.assign(IP);
	}
	this->SetUp();
	/*int isConnected=PingGib(this->gibIP.c_str(),3);
      if (!isConnected)
      {
            DPRINT("WARNING gib seems unreachable");
      }*/
}
#endif
gibccaltDrv::~gibccaltDrv() {
}
int gibccaltDrv::setPulse(int32_t channel,int32_t amplitude,int32_t width,int32_t state) {
    int rc, mysocket;
    std::string cmd;
    char buffer[256];
    if (channel < 0)
      return -1;
    mysocket=OpenSocket(this->gibIP);
    if (mysocket < 0)
    {
      return GIB_UNREACHABLE;
    }
    else
    {
      int value,auxVar;

      value=(state==0) ? 0 : (1 << channel);
      value=0x000FFFFF & ~value;
      if (state==0)
      {
         amplitude=0;
         width=0;
      }
      rc=read(mysocket,buffer,256); //svuotamento buffer
      bzero(buffer,256);
      /********************************************/
      //Setting width and ampl
      cmd=getWriteCommand(0x83D00300,0,convertPulseAmplitude(amplitude));
      rc=write(mysocket,cmd.c_str(),cmd.length());
      bzero(buffer,256);
      rc=read(mysocket,buffer,cmd.length());
      cmd=getWriteCommand(0x83D00300,4,convertPulseWidth(width));
      rc=write(mysocket,cmd.c_str(),cmd.length());
      bzero(buffer,256);
      rc=read(mysocket,buffer,cmd.length());

      /*************Setting channel*******************************/
      cmd=getWriteCommand(0xCEA00004,0,value);
      rc=write(mysocket,cmd.c_str(),cmd.length());
      bzero(buffer,256);
      rc=read(mysocket,buffer,cmd.length());
      DPRINT("Pulse rc = %d\n,buffer = %s",rc,buffer);


      cmd=getCommandToRead(0xCEA00004,0);
      rc=write(mysocket,cmd.c_str(),11);
      bzero(buffer,256);
      rc=read(mysocket,buffer,19);
      auxVar=decodeReadString(buffer);
      if (auxVar == value)
      {
         std::fill(this->ChannelAmplitudes.begin(), this->ChannelAmplitudes.end(), 0);
         std::fill(this->ChannelWidths.begin(), this->ChannelWidths.end(), 0);
         this->ChannelAmplitudes[channel]=amplitude;
         this->ChannelWidths[channel]=width;
         DPRINT("Successfully set Pulse!");
      }
      else
      {
          DPRINT("Error occurred setting Pulse %d read %x input %x\n",rc,auxVar,value);
          close(mysocket);
          shutdown(mysocket,2);
          return -1;
      }
      close(mysocket);
      shutdown(mysocket,2);
    }
	return 0;
}
int gibccaltDrv::setChannelVoltage(int32_t channel,double Voltage) {
  int mysocket,rc;
  char buffer[256];
  mysocket=OpenSocket(this->gibIP);
  if (mysocket < 0)
  {

    return GIB_UNREACHABLE;
  }
  else
  {
    unsigned int ival,address;
    std::string cmd;
    //svuotamento del buffer delle stronzate
    rc = read(mysocket,buffer,256);
    ival=getDACCode(Voltage);
    if (channel != -1)
    {
       address=DACMapAddress[channel];
       cmd=getWriteCommand(address,0,ival);
       rc+=write(mysocket,cmd.c_str(),cmd.length());
       //DPRINT("Launched %s ival %x  at address %x len %d len received %d\n",cmd.c_str(),ival,address,cmd.length(),rc);
       bzero(buffer,256);
       rc+=read(mysocket,buffer,cmd.length());
    }
    else
    {
      //DAC 0 1 2
         #define TIMES 2000
         address=0x83D0003C ;
         cmd=getWriteCommand(address,0,ival);
         rc=write(mysocket,cmd.c_str(),cmd.length());
         bzero(buffer,256);
         rc=read(mysocket,buffer,cmd.length());
         usleep(TIMES);
         address=0x83D0013C;
         cmd=getWriteCommand(address,0,ival);
         rc=write(mysocket,cmd.c_str(),cmd.length());
         bzero(buffer,256);
         rc=read(mysocket,buffer,cmd.length());
         DPRINT("SET. rc=%d\n",rc);
         usleep(TIMES);
         address=0x83D0023C ;
         cmd=getWriteCommand(address,0,ival);
         rc=write(mysocket,cmd.c_str(),cmd.length());
         DPRINT("SET. rc=%d\n",rc);
         bzero(buffer,256);
         rc=read(mysocket,buffer,cmd.length());
         DPRINT("SET. rc=%d\n",rc);
    }
    close(mysocket);
    shutdown(mysocket,2);
  }

	return 0;
}
int gibccaltDrv::PowerOn(int32_t on_state) {
	//int ret= PingGib(this->gibIP.c_str(),4);
  int mysocket,rc;
  char buffer[256];
  mysocket=OpenSocket(this->gibIP);
  if (mysocket < 0)
  {
    return mysocket;
  }
  else
  {
    //svuotamento del buffer delle stronzate
    rc = read(mysocket,buffer,256);
    std::string tmp=getCommandToRead(0xCEA00000,0);
    rc=write(mysocket,tmp.c_str(),tmp.length());
    bzero(buffer,256);
    rc=read(mysocket,buffer,19);
    if (rc != 19)
    {
      DPRINT("Error read state Command not Acknowledged\n");
      close(mysocket);
      shutdown(mysocket,2);
      return -1;
    }
    int actState=decodeReadString(buffer);
    //char* statComm;
    if (on_state != 0)
    {
      on_state=0x100;
      actState |= on_state;
      //statComm="wCEA0000000000108\r\n";
    }
    else
    {
      on_state=0xFFFFFEFF;
      actState &= on_state;
      //statComm="wCEA0000000000008\r\n";
    }
    DPRINT("new state is %x\n",actState);
    std::string theCommand=getWriteCommand(0xCEA00000,0,actState);
    size_t len=theCommand.length();
    //DPRINT("Sending %s (len %d)\n",statComm,strlen(statComm));
    //DPRINT("statComm %s (%d) - tmp %s (%d) equals= %d",statComm,strlen(statComm),theCommand.c_str(),len,!strcmp((const char*)statComm,theCommand.c_str()));
    //rc=write(mysocket,statComm,strlen(statComm));
    rc=write(mysocket,theCommand.c_str(),len);
    bzero(buffer,256);
    rc=read(mysocket,buffer,len);
    if (rc != 19) {
      DPRINT("Error Command not Acknowledged rc is %d\n",rc);
      close(mysocket);
      shutdown(mysocket,2);
      return -2;
    }
    close(mysocket);
    shutdown(mysocket,2);
    return 0;
  }


}
int gibccaltDrv::getState(int32_t* state,std::string& desc) {
    
    int ret=ReadGib();
    if (ret >=0)
    {
      *state=internalState;
      desc=DecodeStatus();
      return 0;
    }
    return ret;
}
int gibccaltDrv::getVoltages(std::vector<double>& voltages) {
    int ret=ReadGib();
    
    if (ret >=0)
    {
      voltages=this->ChannelVoltages;
      return 0;
    }
	return ret;
}
int gibccaltDrv::getNumOfChannels(int32_t* numOfChannels) {
	*numOfChannels=24;
	return 0;
}
int gibccaltDrv::getPulsingState(std::vector<int32_t>& amplitudes,std::vector<int32_t>& widthChannels) {
    int ret=ReadGib();
    if (ret >=0)
    {
      amplitudes=this->ChannelAmplitudes;
      widthChannels=this->ChannelWidths;
      return 0;
    }
	  return ret;
}
int gibccaltDrv::getSupplyVoltages(double* HVSupply,double* P5V,double* N5V) {
  int ret= ReadGib();
  
  if (ret >=0)
  {
    *HVSupply=this->SupplyHV;
    *P5V=this->Pos5;
    *N5V= - this->Neg5;
    return 0;
  }
	return ret;
}
