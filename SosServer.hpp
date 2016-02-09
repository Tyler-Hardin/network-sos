/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * A server class for the SOS game. Note: I changed the name b/c that crap was hella long. Since
 * we're only doing a multiuser one, there is little reason to include it in the name.
 */

#ifndef SOSSERVER_HPP
#define SOSSERVER_HPP

#include "Game.hpp"
#include "Session.hpp"

#include <boost/asio.hpp>

#include <list>
#include <memory>
#include <string>
#include <thread>

class Game;

/**
 * Represents a multiuser server for the game "SOS".
 */
class SosServer : public MessageListener, public MessageSource {
	/** Keep a list of our threads so we can join them when we're destructed. */
	std::list<std::thread> threads;
	
	/** Keep a list of our sessions so we can end/free them when we're destructed. */
	std::list<std::shared_ptr<class Session>> sessions;
	
	/** io_service for creating acceptor */
	boost::asio::io_service io_service;

	/** port to listen on */
	int port;
		
	/** the game/board state */
	Game game;
	
protected:
	/**
	 * Helper function for when the game ends. Prints scores etc.
	 */
	void game_over();
	
public:
	/**
	 * Create a server and set the size of board for any games on this server.
	 *
	 * @param port		the port to listen on
	 * @param size		the size of board this server will use
	 */
	SosServer(int port, int size) : threads(), sessions(), io_service(), port(port), game(size) {}
	
	/**
	 * Listen for connections and block until error or quit.
	 */
	void listen();
	
	/**
	 * Broadcast a message to all clients.
	 *
	 * @param msg		the message to broadcast
	 */
	void broadcast(std::string msg);
	
	/**
	 * Receive a message from a client.
	 * 
	 * @param msg		the message
	 * @param source	the sender of the message
	 */
	 virtual void messageReceived(std::string msg, class MessageSource* source);
	
	/**
	 * Tell the server that a session has ended (i.e. a client has disconnected).
	 * 
	 * @param source	the session that ended
	 */
	virtual void sourceClosed(class MessageSource* source);
};

#endif // SOSSERVER_HPP
