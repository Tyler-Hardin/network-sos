/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * Session implementation.
 */

#include "Session.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/exception/all.hpp>

#include <cstdlib>
#include <iostream>

Session::Session() : io_service(), _socket(io_service), _name("") {}

void Session::do_read(){
	// Create a new buffer for this read.
	boost::asio::streambuf* buf = new boost::asio::streambuf;
	
	// Read until we get a tab.
	boost::asio::async_read_until(_socket, *buf, "\r\n", [this, buf]( 
		boost::system::error_code ec,
		std::size_t bytes){
	
		if(ec == boost::system::errc::broken_pipe || 
		   ec == boost::system::errc::connection_reset || 
		   ec == boost::asio::error::eof){
			// Connection closed.
			
			// End the io_service event loop (i.e. make io_service.run() return) and let this 
			// thread die.
			io_service.stop();
		}
		else if(ec){
			// Some error other than a disconnect. Not expected, so let it be fatal.
			throw boost::system::system_error(ec);
		}
		else{
			// Otherwise, continue on...
		
			// Read message into string.
			std::istream data(buf);
			std::istreambuf_iterator<char> beg_it(data);
			std::istreambuf_iterator<char> end_it;
			std::string s(beg_it, end_it);
			
			boost::erase_all(s, "\r");
			
			// Delete buffer.
			delete buf;
		
			// Send message to listeners.
			this->notifyReceipt(s);
		
			// Set up next read.
			this->do_read();
		}
	});
};

void Session::do_write(std::string msg){
	// Ensure mutual exclusion, so writes aren't interleaved.
	static std::mutex m;
	static std::lock_guard<std::mutex> lock(m);
	
	boost::system::error_code ec;
	boost::asio::write(_socket, boost::asio::buffer(msg + "\r\n"), ec);
	
	if(ec == boost::system::errc::broken_pipe || 
	   ec == boost::system::errc::connection_reset){
		// Notice that this ends the io_service.run() call and deletes the session.
		io_service.stop();
	}
	else if(ec){
		throw boost::system::system_error(ec);
	}
}

void Session::start(){
	// Create a shared point to make sure this object isn't destructed until this function (and the
	// thread for this session) exits.
	std::shared_ptr<Session> _this = shared_from_this();
	
	// Set up the first async read.
	do_read();
	
	try{
		// Tell this thread to handle async stuff for the sessions io_service.
		io_service.run();
	}
	catch(boost::system::system_error e){
		std::cerr << e.what() << std::endl;
		exit(2);
	}
	
	// The only way we get here is if the client disconnects. Tell the listeners.
	closeMessageSource();
	
	// Close the socket.
	_socket.close();
}

void Session::messageReceived(std::string msg, class MessageSource* source){
	send(msg);
}

void Session::sourceClosed(MessageSource* source){
	// We do nothing???
	// This could either mean the server or the client is closing. Either way, the 
	// session/connection will just end.
}

void Session::send(std::string msg){
	//std::cout << "Send: " << msg << std::endl;
	do_write(msg);
}

void Session::close(){
	io_service.stop();
}

tcp::socket& Session::socket(){
	return _socket;
}

std::string Session::getName(){
	return _name;
}

void Session::setName(std::string name){
	_name = name;
}
