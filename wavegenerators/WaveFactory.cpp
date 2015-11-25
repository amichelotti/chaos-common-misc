/* 
 * File:   WaveFactory.cpp
 * Author: michelo
 * 
 * Created on November 25, 2015, 12:05 PM
 */

#include "WaveFactory.h"
#include "RndWave.h"
#include <exception>
#include <stdexcept>
using namespace common::misc::wavegenerators;
#define ADD_GENERATOR(c,g,par) {\
WaveBase* p =new c ();\
if (p->init(par)!=0) throw std::logic_error("bad parameters for generator:"+p->infoParams() );\
if ( # c == g)return WaveBase_t(p); }

WaveFactory::WaveFactory() {
}

 WaveBase_t WaveFactory::getGenerator(const std::string& gen, const std::string& params){
     ADD_GENERATOR(RndWave,gen,params);
     throw std::logic_error("no generator found:\"" + gen+"\"");
 }

WaveFactory::WaveFactory(const WaveFactory& orig) {
}

WaveFactory::~WaveFactory() {
}

