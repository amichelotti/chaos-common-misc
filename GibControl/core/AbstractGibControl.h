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
		class AbstractGibControl {
		  public:
			AbstractGibControl() {};
			virtual ~AbstractGibControl() {};
			virtual int init(void*)=0;
			virtual uint64_t getFeatures()=0;
			virtual int setPulse(int32_t channel, int32_t amplitude, int32_t width, int32_t state)=0;
			virtual int setChannelVoltage(int32_t channel, double Voltage)=0;
			virtual int PowerOn(int32_t)=0;
		};
	}
}//common
#endif
