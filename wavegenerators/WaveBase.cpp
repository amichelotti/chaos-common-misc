/* 
 * File:   WaveBase.cpp
 * Author: michelo
 * 
 * Created on November 25, 2015, 8:44 AM
 */

#include "WaveBase.h"

using namespace common::misc::wavegenerators;

WaveBase::WaveBase() {
    tim=0;
}

WaveBase::WaveBase(const WaveBase& orig) {
}

WaveBase::~WaveBase() {
}

void WaveBase::setTime(uint64_t t){
    tim=t;
}
uint64_t WaveBase::getTime(){return tim;}