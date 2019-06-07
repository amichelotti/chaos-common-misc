#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <stdint.h>
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
	
		
public:
	
	HTTPClient(std::string address, std::string port);
	
	HTTPResponse SendHttpPost(std::string api, std::string contentType, std::string payload);
	
    };
}
}
