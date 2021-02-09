#include "HttpSender.h"
#include <boost/asio.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using boost::asio::ip::tcp;




general::utility::HTTPResponse::HTTPResponse()
{
    ReturnCode=0;
    ReturnMessage="";
    PayloadAnswer="";
}


void general::utility::HTTPClient::write_stream( boost::asio::streambuf& line,boost::posix_time::time_duration timeout)
{
	std::string  data((std::istreambuf_iterator<char>(&line)), std::istreambuf_iterator<char>());
	// Set a deadline for the asynchronous operation. Since this function uses
	// a composed operation (async_write), the deadline applies to the entire
	// operation, rather than individual writes to the socket.
	deadline_.expires_from_now(timeout);

	// Set up the variable that receives the result of the asynchronous
	// operation. The error code is set to would_block to signal that the
	// operation is incomplete. Asio guarantees that its asynchronous
	// operations will never fail with would_block, so any other value in
	// ec indicates completion.
	boost::system::error_code ec = boost::asio::error::would_block;

	// Start the asynchronous operation itself. The boost::lambda function
	// object is used as a callback and will update the ec variable when the
	// operation completes. The blocking_udp_client.cpp example shows how you
	// can use boost::bind rather than boost::lambda.
	boost::asio::async_write(socket_, boost::asio::buffer(data), boost::lambda::var(ec) = boost::lambda::_1);

	// Block until the asynchronous operation has completed.
	do io_service_.run_one(); while (ec == boost::asio::error::would_block);

	if (ec)
		throw boost::system::system_error(ec);
}


std::string general::utility::HTTPClient::get_answer(boost::posix_time::time_duration timeout)
{
	// Set a deadline for the asynchronous operation. Since this function uses
	// a composed operation (async_read_until), the deadline applies to the
	// entire operation, rather than individual reads from the socket.
	deadline_.expires_from_now(timeout);

	// Set up the variable that receives the result of the asynchronous
	// operation. The error code is set to would_block to signal that the
	// operation is incomplete. Asio guarantees that its asynchronous
	// operations will never fail with would_block, so any other value in
	// ec indicates completion.
	boost::system::error_code ec = boost::asio::error::would_block;

	// Start the asynchronous operation itself. The boost::lambda function
	// object is used as a callback and will update the ec variable when the
	// operation completes. The blocking_udp_client.cpp example shows how you
	// can use boost::bind rather than boost::lambda.
	boost::asio::async_read_until(socket_, input_buffer_, "\r\n", boost::lambda::var(ec) = boost::lambda::_1);

	// Block until the asynchronous operation has completed.
	do io_service_.run_one(); while (ec == boost::asio::error::would_block);

	if (ec)
		throw boost::system::system_error(ec);

	std::string line;
	std::istream is(&input_buffer_);
	std::getline(is, line);
	return line;
}



general::utility::HTTPClient::HTTPClient(std::string address, std::string port)
	: socket_(io_service_),
	deadline_(io_service_)
{
	
    host_address = address;
    host_port = port;
	
	deadline_.expires_at(boost::posix_time::pos_infin);

	// Start the persistent actor that checks for deadline expiry.
	check_deadline();
    
}



void general::utility::HTTPClient::connect(const std::string& host, const std::string& service, boost::posix_time::time_duration timeout)
{
	// Resolve the host name and service to a list of endpoints.
	tcp::resolver::query query(host, service);
	tcp::resolver::iterator iter = tcp::resolver(io_service_).resolve(query);

	// Set a deadline for the asynchronous operation. As a host name may
	// resolve to multiple endpoints, this function uses the composed operation
	// async_connect. The deadline applies to the entire operation, rather than
	// individual connection attempts.
	deadline_.expires_from_now(timeout);

	// Set up the variable that receives the result of the asynchronous
	// operation. The error code is set to would_block to signal that the
	// operation is incomplete. Asio guarantees that its asynchronous
	// operations will never fail with would_block, so any other value in
	// ec indicates completion.
	boost::system::error_code ec = boost::asio::error::would_block;

	// Start the asynchronous operation itself. The boost::lambda function
	// object is used as a callback and will update the ec variable when the
	// operation completes. The blocking_udp_client.cpp example shows how you
	// can use boost::bind rather than boost::lambda.
	boost::asio::async_connect(socket_, iter, boost::lambda::var(ec) = boost::lambda::_1);

	// Block until the asynchronous operation has completed.
	do io_service_.run_one(); while (ec == boost::asio::error::would_block);

	// Determine whether a connection was successfully established. The
	// deadline actor may have had a chance to run and close our socket, even
	// though the connect operation notionally succeeded. Therefore we must
	// check whether the socket is still open before deciding if we succeeded
	// or failed.
	if (ec || !socket_.is_open())
		throw boost::system::system_error(
			ec ? ec : boost::asio::error::operation_aborted);
}
general::utility::HTTPResponse general::utility::HTTPClient::SendHttpPost(std::string api, std::string contentType, std::string payload, boost::posix_time::time_duration timeout)
	{
		/*boost::asio::io_service   io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(this->host_address, this->host_port);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);*/
		
		HTTPResponse Resp;
		try
		{
			this->connect(this->host_address, this->host_port, timeout);
			
		}
		catch (std::exception eccezione)
		{
			std::stringstream ss;
			ss<<"Failed to connect to endpoint:"<<eccezione.what();
			Resp.ReturnCode = 404;
			Resp.ReturnMessage = ss.str();
			Resp.PayloadAnswer = "";
			return Resp;
		}
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
		this->write_stream(request, timeout);
		//boost::asio::write(socket_,request);
		boost::asio::streambuf response;
		std::string answ=this->get_answer(timeout);
		
		//boost::asio::read_until(socket_,response,"\r\n");
		//std::istream response_stream(&response);

		std::istringstream resp_str(answ);
		std::istream &response_stream=  resp_str;
		
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code=210;
		std::string status_message;
		

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
		socket_.shutdown(boost::asio::socket_base::shutdown_both);
		return Resp;



		




	}
void general::utility::HTTPClient::check_deadline()
{
	
	// Check whether the deadline has passed. We compare the deadline against
	// the current time since a new asynchronous operation may have moved the
	// deadline before this actor had a chance to run.
	if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
	{
		// The deadline has passed. The socket is closed so that any outstanding
		// asynchronous operations are cancelled. This allows the blocked
		// connect(), read_line() or write_line() functions to return.
		boost::system::error_code ignored_ec;
		socket_.close(ignored_ec);

		// There is no longer an active deadline. The expiry is set to positive
		// infinity so that the actor takes no action until a new deadline is set.
		deadline_.expires_at(boost::posix_time::pos_infin);
	}

	// Put the actor back to sleep.
	deadline_.async_wait(boost::lambda::bind(&::general::utility::HTTPClient::check_deadline, this));
	
}



//----------------------------------------------------------------------


