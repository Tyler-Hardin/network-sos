/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * Client interface.
 */

#ifndef SOSCLIENT_HPP
#define SOSCLIENT_HPP

#include "MessageListener.hpp"
#include "MessageSource.hpp"
#include "Session.hpp"

#include <memory>
#include <string>

/**
 * Represents a client for the SOS server.
 */
class SosClient : public MessageListener, public MessageSource{
	/** session that handles the connection */
	std::shared_ptr<Session> session;
	
	/** thread that the session will use */
	std::thread session_thread;
	
	/** hostname of server */
	std::string host;
	
	/** port to connect to */
	int port;
	
	/** username */
	std::string name;
	
	/** if we're connected */
	bool connected = false;
	
public:
	/**
	 * Creates a client object that will connect to the server.
	 *
	 * @param host		the host to connect to
	 * @param port		the port to connect to
	 * @param name		the username to use
	 */
	SosClient(std::string host, int port, std::string name) : host(host), port(port), name(name) {}
	
	
	/**
	 * Hand control of the thread to the client. Blocks until disconnect or error.
	 */
	void run();
	
	/**
	 * Receive data from the session.
	 *
	 * @param message	the message the server sent
	 * @param source	the session
	 */
	void messageReceived(std::string message, class MessageSource* source);
	
	/**
	 * Set connected to false so the client will exit.
	 * 
	 * @param source	the source that disconnected
	 */
	void sourceClosed(class MessageSource* source);
};

#endif // SOSCLIENT_HPP
