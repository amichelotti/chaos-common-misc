#include "HttpSender.h"
#include <boost/asio.hpp>
#include <stdint.h>
using boost::asio::ip::tcp;

general::utility::HTTPResponse::HTTPResponse()
{
    ReturnCode=0;
    ReturnMessage="";
    PayloadAnswer="";
}
general::utility::HTTPClient::HTTPClient(std::string address, std::string port)
{
    
    host_address = address;
    host_port = port;
    
}
general::utility::HTTPResponse general::utility::HTTPClient::SendHttpPost(std::string api, std::string contentType, std::string payload)
	{
		boost::asio::io_service   io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(this->host_address, this->host_port);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::socket socketb(io_service);
		boost::asio::connect(socketb, endpoint_iterator);
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		std::stringstream tmp;
		std::string firstLine = "POST ";
		if (api[0] == '/')
			firstLine += api;
		else
			firstLine += "/" + api;
		firstLine += " HTTP/1.1\r\n";

		request_stream << firstLine;
		tmp << firstLine;
		request_stream << "HOST:"<<this->host_address<<"\r\n";
		tmp << "HOST:"<<this->host_address<<"\r\n";
		request_stream << "User-Agent: C/1.0\r\n";
		tmp << "User-Agent: C/1.0\r\n";
		if (contentType == "")
		{
			contentType = "text/html;";
		}
		if (contentType[contentType.length() - 1] != ';')
			contentType += ";";
		request_stream << "Content-Type: "<<contentType<<"\r\n";
		tmp << "Content-Type: " << contentType << "\r\n";
		request_stream << "Accept: */*\r\n";
		tmp << "Accept: */*\r\n";
		request_stream << "Content-Length: " << payload.length() << "\r\n";
		tmp << "Content-Length: " << payload.length() << "\r\n";
		request_stream << "Connection: close\r\n\r\n";
		tmp << "Connection: close\r\n\r\n";
		request_stream << payload;
		tmp << payload;
		boost::asio::write(socketb,request);
		boost::asio::streambuf response;
		boost::asio::read_until(socketb,response,"\r\n");
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code=210;
		std::string status_message;
		HTTPResponse Resp;

		std::getline(response_stream,status_message);
		if (!response_stream || http_version.substr(0,5)!="HTTP/")
		{
			std::cout << "Invalid response\n";
			return Resp;
		}
		else
		{
			Resp.ReturnMessage=status_message;
			if (sscanf(status_message.c_str(), "%d", &status_code) == 1)
			{
				Resp.ReturnCode=status_code;
			}
			while (std::getline(response_stream, status_message))
			{
				Resp.PayloadAnswer+= status_message;
			}
		}
		
		
		socketb.shutdown(boost::asio::socket_base::shutdown_both);
		return Resp;



		




	}
