/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * Client implementation.
 */

#include "SosClient.hpp"

#include <iostream>

void SosClient::run(){
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	
	// Perform DNS query.
	tcp::resolver::query query(host, std::to_string(port));
	
	// Get iterators to for the list of DNS entries.
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;
	
	// Create a session to manage the connect.
	session.reset(new Session);
	
	// Get reference to the session's socket.
	tcp::socket& socket = session->socket();
	
	// Iterator over entries until connect works.
	boost::system::error_code ec = boost::asio::error::host_not_found;
	while(ec && endpoint_iterator != end){
		socket.close();
		socket.connect(*endpoint_iterator++, ec);
	}
	if(ec)
		throw boost::system::system_error(ec);
	
	// At this point, a server is connected. (B/c the throw didn't happen.)
	connected = true;
	
	// Interconnect listeners.
	addMessageListener(session.get());
	session->addMessageListener(this);
	
	// Start thread to manage session.
	session_thread = std::thread(std::bind(&Session::start, session));
	
	session->send("/connect " + name);
	
	// This really is a forever loop, since it will always exit before connected == false.
	while(connected){
		std::string command;
		std::getline(std::cin, command);
		notifyReceipt(command);
	}
	closeMessageSource();
	session_thread.join();
}

void SosClient::messageReceived(std::string message, class MessageSource* source){
	std::cout << message;
}

void SosClient::sourceClosed(class MessageSource* source){
	std::cout << "Connection closed." << std::endl;
	
	// It's dirty, but we need to exit here because the main thread is still blocked reading from
	// STDIN at line 51 and there's no way to interrupt it in C++. Exceptions can't be sent from one
	// thread to another.
	exit(0);
	
	connected = false;
}
