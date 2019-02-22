/*
HetPicSim.h
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
#ifndef __HETPic__HetPicSim__
#define __HETPic__HetPicSim__
#include <common/misc/HETPic/core/AbstractHETPic.h>
#include <vector>
#ifdef CHAOS
#include <chaos/common/data/CDataWrapper.h>
#endif
namespace common {
	namespace hetpic {
		namespace models {
			class HetPicSim: public AbstractHETPic {
				public:
				HetPicSim(const std::string Parameters);
				#ifdef CHAOS
				HetPicSim(const chaos::common::data::CDataWrapper& config);
				#endif
				~HetPicSim();
				int SetHighThreshold(int32_t channel,int32_t millivolts);
				int SetLowThreshold(int32_t channel,int32_t millivolts);
				int setPulse(int32_t value);
				int getNumberOfChannel(int32_t& chanNum);
				int getStatus(int32_t& status);
				int getHighThresholds(std::vector<int32_t>& highThresholds);
				int getLowThresholds(std::vector<int32_t>& lowThresholds);
				private:
				uint32_t numberOfChannels;
				int32_t internalStatus;
				int32_t pulseValue;
				std::vector<int32_t> lowTHR;
				std::vector<int32_t> highTHR;
			};//end class
		}//end namespace models
	}//end namespace hetpic
}//end namespace common
#endif //__HETPic__HetPicSim__
