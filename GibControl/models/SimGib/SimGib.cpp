/*
SimGib.cpp
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
#ifdef CHAOS
#include <chaos/common/data/CDataWrapper.h>
#endif
#include "SimGib.h"
using namespace common::gibcontrol;
using namespace common::gibcontrol::models;
SimGib::SimGib(const std::string Parameters) {
}
#ifdef CHAOS
SimGib::SimGib(const chaos::common::data::CDataWrapper &config) { 
}
#endif
SimGib::~SimGib() {
}
SimGib::init(void*) {
	DPRINT("DRIVER SimGib::init");
	return 0;
}
SimGib::deinit() {
	return 0;
}
uint64_t SimGib::getFeatures() {
	return 0;
}
int SimGib::setPulse(int32_t channel,int32_t amplitude,int32_t width,int32_t state) {
	return 0;
}
int SimGib::setChannelVoltage(int32_t channel,double Voltage) {
	return 0;
}
int SimGib::PowerOn(int32_t on_state) {
	return 0;
}
int SimGib::getState(int32_t* state,std::string& desc) {
	DPRINT("SimGib driver getState");
	*state=21;
	desc="the state is ok";
	return 0;
}
