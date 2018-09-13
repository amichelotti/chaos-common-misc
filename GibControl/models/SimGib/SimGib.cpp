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
	internalState=0;
	pulseStateMask=0;
	DPRINT("obtained string %s",Parameters.c_str());
	
	adcChannels.clear();
	pulsingAmplitudes.clear();
	pulsingWidth.clear();
	for (int i=0; i < this->channels; i++)
	{
	   pulsingAmplitudes.push_back(0);
	   pulsingWidth.push_back(0);
	   adcChannels.push_back(0);
	}
}
#ifdef CHAOS
SimGib::SimGib(const chaos::common::data::CDataWrapper &config) { 
	internalState=0;
	pulseStateMask=0;
	adcChannels.clear();
	pulsingAmplitudes.clear();
	pulsingWidth.clear();
	for (int i=0; i < this->channels; i++)
	{
	   pulsingAmplitudes.push_back(0);
	   pulsingWidth.push_back(0);
	   adcChannels.push_back(0);
	}
	   
}
#endif
SimGib::~SimGib() {
}
int SimGib::init(void* conf) {
	DPRINT("DRIVER SimGib::init");
	internalState=12;
	return 0;
}
int SimGib::deinit(void) {
	return 0;
}
uint64_t SimGib::getFeatures() {
	return 24;
}
int SimGib::setPulse(int32_t channel,int32_t amplitude,int32_t width,int32_t state) {
	DPRINT("Called SetPulse channel %d amlitude %d width %d state %d",channel,amplitude,width,state);
	if (channel >= this->channels)
	 return -1;

	if (state > 0)
	{
		RAISEBIT(pulseStateMask,channel);
		this->pulsingAmplitudes[channel] = amplitude;
		this->pulsingWidth[channel] = width;
	}
	else
	{
		DOWNBIT(pulseStateMask,channel);
		this->pulsingAmplitudes[channel] = 0;
		this->pulsingWidth[channel] = 0;
	}
	if (pulseStateMask != 0)
	{
		UPMASK(internalState,::common::gibcontrol::GIBCONTROL_PULSING);
	}
	else
	{
		DOWNMASK(internalState,::common::gibcontrol::GIBCONTROL_PULSING);
	}


	return 0;
}
	
int SimGib::setChannelVoltage(int32_t channel,double Voltage) {
	DPRINT("Called setChannelVoltage channel %d Voltage %f",channel,Voltage);
	if ((channel > this->channels-1) || (channel < -1))
	{
	   DPRINT(" setChannelVoltage channel %d parameter out of bounds",channel);
	   return -1;
	}
	if (channel > -1)
		adcChannels[channel]=Voltage; 
	else if (channel == -1)
	{
		for (int i=0; i < this->channels;i++)
		{
			adcChannels[i]=Voltage;
		}

	}
	return 0;
}
int SimGib::PowerOn(int32_t on_state) {
	if (on_state == 1)
	   internalState|=1;
	else
	   internalState&=0xFFFFFFFE;
	return 0;
}
int SimGib::getState(int32_t* state,std::string& desc) {
	*state=internalState;
	desc=DescribeState(internalState);
	
	return 0;
}

int SimGib::getVoltages(std::vector<double>& vec )
{
	vec = adcChannels;
	return 0;
}

int SimGib::getNumOfChannels(int32_t* numOfChannels) {
	*numOfChannels = this->channels;
	return 0;
}
int SimGib::getPulsingState(std::vector<int32_t>& amplitudes,std::vector<int32_t>& widthChannels) {
	amplitudes=this->pulsingAmplitudes;
	widthChannels=this->pulsingWidth;
	return 0;
}
int SimGib::getSupplyVoltages(double* HVSupply,double* P5V,double* N5V)
{
	std::srand(std::time(nullptr));
	
	double noise=(double) ((std::rand() % 10)-5) / 10;
	*P5V=5+noise;
	noise= (double) ((std::rand() % 10)-5) / 10;
	*N5V=-5+noise;
	noise=(double) ((std::rand() % 10)-5) / 10;
	if (!(CHECKMASK(internalState,::common::gibcontrol::GIBCONTROL_SUPPLIED)))
	{
		*HVSupply=0;
	}
	else
		*HVSupply=43 + noise ;
	return 0;
}

std::string SimGib::DescribeState(int32_t state)
{
	std::string desc;
	if ((internalState & ::common::gibcontrol::GIBCONTROL_SUPPLIED)== ::common::gibcontrol::GIBCONTROL_SUPPLIED)
		desc="ON ";
	else
		desc="OFF ";
	//if ((internalState & ::common::gibcontrol::GIBCONTROL_PULSING) == ::common::gibcontrol::GIBCONTROL_PULSING)
	if ((internalState & ::common::gibcontrol::GIBCONTROL_PULSING) != 0 )
		desc+="PULSING";
	return desc;

}
