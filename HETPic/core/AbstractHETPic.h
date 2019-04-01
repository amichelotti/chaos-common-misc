/*
AbstractHETPic.h
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
#ifndef __common_AbstractHETPic_h__
#define __common_AbstractHETPic_h__
namespace common {
	namespace hetpic {
		typedef enum {
			HETPIC_OK = 0x1,
			HETPIC_NOT_RESPONSIVE = 0x2,
			HETPIC_PULSING = 0x4			
		} StatusPIC;
		class AbstractHETPic {
		  public:
			AbstractHETPic() {};
			virtual ~AbstractHETPic() {};
			virtual int SetHighThreshold(int32_t channel,int32_t millivolts)=0;
			virtual int SetLowThreshold(int32_t channel,int32_t millivolts)=0;
			virtual int setPulse(int32_t value)=0;
			virtual int getNumberOfChannel(int32_t& chanNum)=0;
			virtual int getStatus(int32_t& status)=0;
			virtual int getHighThresholds(std::vector<int32_t>& highThresholds)=0;
			virtual int getLowThresholds(std::vector<int32_t>& lowThresholds)=0;
		};
	}
}//common
#endif
