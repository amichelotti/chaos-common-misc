/* 
 * File:   RndWave.cpp
 * Author: michelo
 * 
 * Created on November 25, 2015, 9:21 AM
 */
#include <regex>

#include "RndWave.h"
#include <string>
#include <stdlib.h>
using namespace common::misc::wavegenerators;
static std::regex params("([-\\.\\d]+):([-\\.\\d]+)");

RndWave::RndWave() {
    max=min=0;
    srand(time(NULL));
}

RndWave::RndWave(const RndWave& orig) {
}

RndWave::~RndWave() {
}

  
int RndWave::init(const std::string& par){
    std::smatch match;
    
    if(std::regex_match(par,match,params)){
        std::string smin=match[1];
        std::string smax=match[2];
        min=atof(smin.c_str());
        max=atof(smax.c_str());
        return 0;
    }
    
    return -1;
}
  
void RndWave::deinit(){
}
  
void RndWave::reset(){
}
    
  
double RndWave::generate(){
    double ret;
    ret = min + ((rand()*(max-min))/RAND_MAX);
    return ret;
}
    

std::string RndWave::infoParams(){

   std::string p="generate a random number between min:max";
   return p;
}