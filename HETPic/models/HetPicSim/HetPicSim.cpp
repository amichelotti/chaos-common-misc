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
	this->numberOfChannels=32;
	for (int i=0; i < this->numberOfChannels; i++)
	{
		this->lowTHR.push_back(2);
		this->highTHR.push_back(3);
	}
}
#ifdef CHAOS
HetPicSim::HetPicSim(const chaos::common::data::CDataWrapper &config) { 
	this->numberOfChannels=32;
	for (int i=0; i < this->numberOfChannels; i++)
	{
		this->lowTHR.push_back(1);
		this->highTHR.push_back(2);
	}
}
#endif
HetPicSim::~HetPicSim() {
}
int HetPicSim::SetHighThreshold(int32_t channel,int32_t millivolts) {
	if ((channel>=0) && (channel < this->numberOfChannels))
	{
		this->highTHR[channel]=millivolts;
		return 0;
	}
	else if (channel==-1)
	{
		for (int i=0; i < this->numberOfChannels; i++)
		{
			this->highTHR[i]=millivolts;
		}
		return 0;
	}
	else
	{

		return -1;
	}
	
}
int HetPicSim::SetLowThreshold(int32_t channel,int32_t millivolts) {
	if ((channel>=0) && (channel < this->numberOfChannels))
	{
		this->lowTHR[channel]=millivolts;
		return 0;
	}
	else if (channel==-1)
	{
		for (int i=0; i < this->numberOfChannels; i++)
		{
			this->lowTHR[i]=millivolts;
		}
		return 0;
	}
	else
	{

		return -1;
	}
	return 0;
}
int HetPicSim::setPulse(int32_t value) {
	return 0;
}
int HetPicSim::getNumberOfChannel(int32_t& chanNum) {
	chanNum=this->numberOfChannels;
	return 0;
}
int HetPicSim::getStatus(int32_t& status) {
	status= ::common::hetpic::HETPIC_OK;
	return 0;
}
int HetPicSim::getHighThresholds(std::vector<int32_t>& highThresholds) {
	highThresholds.clear();
	for (int i=0; i < this->highTHR.size(); i++)
	{
		highThresholds.push_back(this->highTHR[i]);
	}
	return 0;
}
int HetPicSim::getLowThresholds(std::vector<int32_t>& lowThresholds) {
	lowThresholds.clear();
	for (int i=0; i < this->lowTHR.size(); i++)
	{
		lowThresholds.push_back(this->lowTHR[i]);
	}
	return 0;
}
