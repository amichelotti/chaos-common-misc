/*
SimGib.h
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
#ifndef __GibControl__SimGib__
#define __GibControl__SimGib__
#include <common/misc/GibControl/core/AbstractGibControl.h>
#include <vector>
#ifdef CHAOS
#include <chaos/common/data/CDataWrapper.h>
#endif
namespace common {
	namespace gibcontrol {
		namespace models {
			class SimGib: public AbstractGibControl {
				public:
				SimGib(const std::string Parameters);
#ifdef CHAOS
				SimGib(const chaos::common::data::CDataWrapper& config);
#endif
				~SimGib();
				int init(void*);
				int deinit(void);
				uint64_t getFeatures();
				int setPulse(int32_t channel,int32_t amplitude,int32_t width,int32_t state);
				int setChannelVoltage(int32_t channel,double Voltage);
				int PowerOn(int32_t on_state);
				int getState(int32_t* state,std::string& desc);
				int getVoltages(std::vector<double>& vec );
				private:
				int internalState;
				std::vector<double> adcChannels;
			};//end class
		}//end namespace models
	}//end namespace gibcontrol
}//end namespace common
#endif //__GibControl__SimGib__
