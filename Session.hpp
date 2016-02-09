/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * Manages the connection to a client.
 */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "MessageSource.hpp"
#include "SosServer.hpp"

#include <boost/asio.hpp>

#include <string>

using boost::asio::ip::tcp;

/**
 * Represents the session/state between the server and a single client connection.
 */
class Session : public std::enable_shared_from_this<Session>, 
                public MessageListener,
                public MessageSource {
	/** Each thread (i.e. session) needs its own service. */
	boost::asio::io_service io_service;
	
	/** socket we're listening to */
	tcp::socket _socket;
	
	/** this client's chosen name */
	std::string _name;

protected:
	
	/**
	 * Queues async read from socket and inform listeners when a message is read.
	 */
	void do_read();
	
	/**
	 * Performs sync write to socket.
	 *
	 * @param msg		the message to send
	 */
	void do_write(std::string msg);
	 
public:
	/**
	 * Creates session to manage the connection that will be established on `socket`.
	 */
	Session();
	
	/**
	 * Receive a message. (And, more specifically, send the message to the client.)
	 *
	 * @param msg		the message to send
	 * @param source	the source of the message
	 */
	virtual void messageReceived(std::string msg, class MessageSource* source);
	
	/**
	 * Receive a notification that a source has closed.
	 *
	 * @param source	the source that closed
	 */
	virtual void sourceClosed(class MessageSource* source);
	
	/**
	 * Start managing the connection. Precondition: socket should be connected (Specifically, it 
	 * sets up the first async read.)
	 */
	void start();
	
	/**
	 * Close the connection.
	 */
	void close();
	
	/**
	 * Send a message to this client. If the client is disconnected or other error occurs, we print
	 * a warning (to server STDERR) and silently fail. (B/c otherwise we have to manage inter-thread
	 * exceptions which are dirty and hard: 
	 * http://stackoverflow.com/questions/233127/how-can-i-propagate-exceptions-between-threads.)
	 *
	 * @param msg		the message to send
	 */
	void send(std::string msg);
	
	/**
	 * Accessor for the socket this session manages. (Useful for accepting.)
	 *
	 * @returns this session's socket
	 */
	tcp::socket& socket();
	
	/**
	 * Gets this clients name.
	 *
	 * @returns the name for this client
	 */
	std::string getName();
	
	/**
	 * Sets this clients name.
	 *
	 * @param name		the name for this client
	 */
	void setName(std::string name);
};

#endif // CONNECTION_HPP
