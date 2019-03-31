#include <common/misc/driver/SerialChannelFactory.h>

int main(int argc,char** argv){
  if(argc<2){
    std::cout<<"## you must specify a valid JSON"<<std::endl;
    return -1;
  }
  
  try {
	  common::misc::driver::AbstractSerialChannel_psh channel=common::misc::driver::SerialChannelFactory::getChannelFromJson(std::string(argv[1]));
	  channel->init();
	  while(1){
	 	  char buffer[1024];
	 	  int ret=channel->read(buffer,sizeof(buffer),10000);
	 	  if(ret>0){
	 		  std::string answ=buffer;
	 		  std::cout<<"received "<<ret<<" bytes :\""<<buffer<<"\"\n";
	 		  answ+="-server";
	 		  ret= channel->write((void*)answ.c_str(),strlen(answ.c_str())+1,10000);
	 		  std::cout<<"sending "<<ret<<" bytes\n";

	 	  } else {
	 		  std::cout<<"## an error occurred:"<<ret<<"\n";
	 		  return ret;
	 	  }

	   }
  } catch (std::logic_error e){
	  std::cout<<"## error opening channel:"<<e.what();
	  return -1;
  }


  return 0;
}
