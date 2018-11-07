/*
AbstractGibControl.h
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
#include <inttypes.h>
#include <string>
#ifndef __common_AbstractGibControl_h__
#define __common_AbstractGibControl_h__
namespace common {
	namespace gibcontrol {
#define UPMASK(bitmask,powerofTwo) \
bitmask |= powerofTwo 

#define DOWNMASK(bitmask,powerofTwo) \
bitmask=bitmask & (~powerofTwo) 

#define CHECKMASK(bitmask,powerofTwo) \
((bitmask & powerofTwo) != 0)

#define RAISEBIT(bitmask,bitToRaise) \
bitmask |= (1 << bitToRaise) 


#define DOWNBIT(bitmask,bitToLower) \
{ int mask = ~(1 << bitToLower); \
  bitmask=bitmask & mask; }

#define CHECKBIT(bitmask,bitToCheck)\
((bitmask & (1 << bitToCheck)) != 0) 






typedef enum {
			GIBCONTROL_SUPPLIED = 0x1,
			GIBCONTROL_PULSING = 0x2,
			GIBCONTROL_UNKNOWN = 0x4
		} GibStatus;
		class AbstractGibControl {
		  public:
			AbstractGibControl() {};
			virtual ~AbstractGibControl() {};
			//virtual int init(void*)=0;
			//virtual uint64_t getFeatures()=0;
			virtual int setPulse(int32_t channel,int32_t amplitude,int32_t width,int32_t state)=0;
			virtual int setChannelVoltage(int32_t channel,double Voltage)=0;
			virtual int PowerOn(int32_t on_state)=0;
			virtual int getState(int32_t* state,std::string& desc)=0;
			virtual int getVoltages(std::vector<double>& voltages)=0;
			virtual int getNumOfChannels(int32_t* numOfChannels)=0;
			virtual int getPulsingState(std::vector<int32_t>& amplitudes,std::vector<int32_t>& widthChannels)=0;
			virtual int getSupplyVoltages(double* HVSupply,double* P5V,double* N5V)=0;
		};
	}
}//common
#endif
