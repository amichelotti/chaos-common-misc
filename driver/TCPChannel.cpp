/*
 * TCPChannel.cpp
 *
 *  Created on: Sep 22, 2017
 *      Author: michelo
 */

#include "TCPChannel.h"
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/algorithm/string.hpp>
#include <common/debug/core/debug.h>
using boost::lambda::var;

using boost::lambda::_2;
namespace common {
namespace misc {
namespace driver {


TCPChannel::TCPChannel(const std::string& _ip_port):AbstractChannel(_ip_port),socket(io_service),deadline(io_service)  {
	// TODO Auto-generated constructor stub
	std::vector<std::string> strs;
	boost::split(strs, _ip_port, boost::is_any_of(":"));
	if(strs.size()!=2){
		throw std::logic_error("bad IP:port specification");
	}
	ip=strs[0];
	port=atoi(strs[1].c_str());
	deadline.expires_at(boost::posix_time::pos_infin);
}

TCPChannel::~TCPChannel() {
	// TODO Auto-generated destructor stub
}


int TCPChannel::init(){
    boost::system::error_code error = boost::asio::error::host_not_found;
    socket.connect( asio::ip::tcp::endpoint( asio::ip::address::from_string(ip), port ),error );
    if(error){
    	DPRINT("Connecting %s:%d error:%s",ip.c_str(),port,error.message().c_str());
    	return -1;
    }
	return 0;
}


 int TCPChannel::deinit(){
	 socket.close();

	 return 0;
 }

 int TCPChannel::read(void *buff,int nb,int ms_timeo,int*timeout_arised){
	 if(socket.is_open()){
		  boost::system::error_code ec = boost::asio::error::would_block;

		   boost::system::error_code error;
		   int ret;
		   if(ms_timeo>0){

			    deadline.expires_from_now(boost::posix_time::milliseconds(ms_timeo));

		   }
		   ret= asio::async_read( socket,  boost::asio::buffer(buff, nb), asio::transfer_all(), var(ec,ret) = _2 );
		   do io_service.run_one(); while (ec == boost::asio::error::would_block);


		   if(ec){
			   DERR("Error reading %p, len %d, error:%s",buf,nb,ec.message().c_str());
			   return -1;
		   }
		   return ret;
	}
		   DERR("socket is closed");
		   return -2;
 }


  int TCPChannel::read_async(void *buffer,int nb){

	  return 0;
  }


   int TCPChannel::byte_available_read(){

	   return 0;
   }


   int TCPChannel::write(void *buffer,int nb,int ms_timeo=0,int*timeout_arised){
	   if(socket.is_open()){
		   boost::system::error_code error;
		   int ret;
		   ret=asio::write(socket,asio::buffer(buffer,nb),error);
		   if(error){
			   DERR("Error writing %p, len %d, error:%s",buffer,nb,error.message().c_str());
			   return -1;
		   }
		   return ret;
	   }
	   DERR("socket is closed");
	   return -2;
   }


   int TCPChannel::write_async(void *buffer,int nb){
	   return 0;
   }



   int TCPChannel::byte_available_write(){
	   return 0;
   }

   void TCPChannel::flush_write(){
	   return ;
   }


   void TCPChannel::flush_read(){
	   return;
   }




} /* namespace driver */
} /* namespace misc */
} /* namespace common */
