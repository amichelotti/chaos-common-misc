#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <stdint.h>
#include <boost/asio.hpp>


namespace general
{
    namespace utility
    {
        class HTTPResponse
        {
            public:
            int32_t ReturnCode;
              std::string ReturnMessage;
              std::string PayloadAnswer;
            HTTPResponse() ;
        };


class HTTPClient
{
private :
	  std::string host_address;
	  std::string host_port;
	  boost::asio::io_service io_service_;
	  boost::asio::ip::tcp::socket socket_;
	  boost::asio::deadline_timer deadline_;
	  boost::asio::streambuf input_buffer_;
	  void check_deadline();
	  void write_stream(  boost::asio::streambuf& line, boost::posix_time::time_duration timeout);
	  std::string get_answer(boost::posix_time::time_duration timeout);
	  void connect(const std::string& host, const std::string& service, boost::posix_time::time_duration timeout);
		
public:
	
	HTTPClient(std::string address, std::string port);
	
	HTTPResponse SendHttpPost(std::string api, std::string contentType, std::string payload,boost::posix_time::time_duration timeout=boost::posix_time::seconds(2));
	
    };





}
}
