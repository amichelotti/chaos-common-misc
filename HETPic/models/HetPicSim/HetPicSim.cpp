/*
HetPicSim.cpp
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
#include "HetPicSim.h"
using namespace common::hetpic;
using namespace common::hetpic::models;
HetPicSim::HetPicSim(const std::string Parameters) {
}
#ifdef CHAOS
HetPicSim::HetPicSim(const chaos::common::data::CDataWrapper &config) { 
}
#endif
HetPicSim::~HetPicSim() {
}
int HetPicSim::SetHighThreshold(int32_t channel,int32_t millivolts) {
	return 0;
}
int HetPicSim::SetLowThreshold(int32_t channel,int32_t millivolts) {
	return 0;
}
int HetPicSim::setPulse(int32_t value) {
	return 0;
}
int HetPicSim::getLowThresholds(int32_t& lowthresholds) {
	return 0;
}
int HetPicSim::getHighThresholds(int32_t& highthresholds) {
	return 0;
}
int HetPicSim::getTemperatures(double& temperature_0,double& temperature_1) {
	return 0;
}
