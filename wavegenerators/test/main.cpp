// Test of Wave generator libraries
//  main.cpp
// Andrea Michelotti
#include <boost/program_options.hpp>
#include <common/misc/wavegenerators/WaveFactory.h>
#include <iostream>
#include "WaveFactory.h"



int main(int argc, char *argv[])
{
  std::string wave;
  int samples=10;
  boost::program_options::options_description desc("options");
  desc.add_options()("help", "help");
  desc.add_options()("wave", boost::program_options::value<std::string>(&wave),"the wavegenerator string one the format:<wavegenerator class>:<wave generator initialization>,i.e RndWave:0.10:20.2 generates random numbers from .10 to 20.2");
  desc.add_options()("samples", boost::program_options::value<int>(&samples)->default_value(10),"number of samples to create");
  // put your additional options here

  //////
  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc,argv, desc),vm);
  boost::program_options::notify(vm);
  
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }  
  if(vm.count("wave")==0){
    std::cerr << "# You must specify a valid generator initialization string"<<std::endl;
    return -1;
  }
  try {
  common::misc::wavegenerators::WaveBase_t wav=common::misc::wavegenerators::WaveFactory::getGenerator(wave);
  while(samples--){
    std::cout<<wav->generate()<<std::endl;
  }
  } catch(std::exception e){
    std::cerr<<"## error: "<< e.what()<<std::endl;
    return -2;
  }
  return 0;
}
