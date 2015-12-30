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
#include <boost/regex.hpp>
using namespace common::misc::wavegenerators;
static boost::regex drv_opt("([a-zA-Z]+):(.+)");
#define ADD_GENERATOR(c,g,par) {		\
WaveBase* p =new c ();\
if (p->init(par)!=0) throw std::logic_error("bad parameters for generator:"+p->infoParams() );\
if ( # c == g)return WaveBase_t(p); }

WaveFactory::WaveFactory() {
}

 WaveBase_t WaveFactory::getGenerator(const std::string& params){
   boost::smatch match;
   if(boost::regex_match(params,match,drv_opt,boost::match_extra)){
     std::string gen,par;
     gen=match[1];
     par=match[2];
     ADD_GENERATOR(RndWave,gen,par);
   } else {
     throw std::logic_error("bad format for getting generator:\""+params+"\" expected <generator class>:<class initializer>");
   }

   throw std::logic_error("no generator found:\"" + params+"\"");
 }

WaveFactory::WaveFactory(const WaveFactory& orig) {
}

WaveFactory::~WaveFactory() {
}

