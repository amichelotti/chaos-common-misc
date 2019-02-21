/*
gibccaltDrv.h
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
#ifndef __GibControl__gibccaltDrv__
#define __GibControl__gibccaltDrv__
#include <common/misc/GibControl/core/AbstractGibControl.h>
#include <vector>
//added for tcp
#include <sys/ioctl.h>
#include <sys/select.h>
#include <string.h>
#include "strings.h"
#include "unistd.h"
#include <arpa/inet.h>
#include <netdb.h>

#ifdef CHAOS
#include <chaos/common/data/CDataWrapper.h>
#endif
namespace common {
	namespace gibcontrol {
		namespace models {
			class gibccaltDrv: public AbstractGibControl {
				public:
				gibccaltDrv(const std::string Parameters);
#ifdef CHAOS
				gibccaltDrv(const chaos::common::data::CDataWrapper& config);
#endif
				~gibccaltDrv();
				int setPulse(int32_t channel,int32_t amplitude,int32_t width,int32_t state);
				int setChannelVoltage(int32_t channel,double Voltage);
				int PowerOn(int32_t on_state);
				int getState(int32_t* state,std::string& desc);
				int getVoltages(std::vector<double>& voltages);
				int getNumOfChannels(int32_t* numOfChannels);
				int getPulsingState(std::vector<int32_t>& amplitudes,std::vector<int32_t>& widthChannels);
				int getSupplyVoltages(double* HVSupply,double* P5V,double* N5V);

				private:
				std::string gibIP;
				int32_t numOfChannels;
				unsigned int lastUpdate;
				unsigned int pulsingstate;
				unsigned int internalState;
				double  SupplyHV;
				double Neg5,Pos5;
				double Temperature[2];
				std::vector<double> ChannelVoltages;
				std::vector<int32_t> ChannelAmplitudes;
				std::vector<int32_t> ChannelWidths;
				void SetUp();
				int ReadGib(int numOfTryes=0);
				void SetInternalState();
				std::string DecodeStatus();

				const unsigned int DACMapAddress[24]={ 0x83D00000,0x83D00004,
                                        0x83D00100,0x83D00104,
                                        0x83D00200,0x83D00204,
                                        0x83D00008,0x83D0000C,
                                        0x83D00108,0x83D0010C,
                                        0x83D00208,0x83D0020C,
                                        0x83D00010,0x83D00014,
                                        0x83D00110,0x83D00114,
                                        0x83D00210,0x83D00214,
                                        0x83D00018,0x83D0001C,
                                        0x83D00118,0x83D0011C,
                                        0x83D00218,0x83D0021C};
				const int VettoreMapSipm[24]={0,1,6,7,12,13,18,19,2,3,8,9,14,15,20,21,4,5,10,11,16,17,22,23};

			};//end class
		}//end namespace models
	}//end namespace gibcontrol
}//end namespace common
#endif //__GibControl__gibccaltDrv__