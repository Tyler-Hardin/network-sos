/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * MessageListener interface (actually an abstract class).
 */

#ifndef MESSAGELISTENER_HPP
#define MESSAGELISTENER_HPP

#include "MessageSource.hpp"

#include <string>

/**
 * Represents a listener.
 */
class MessageListener {
public:
	
	/**
	 * Sends a message to this listener.
	 *
	 * @param message	the message
	 * @param source	the source that sent the message
	 */
	virtual void messageReceived(std::string message, class MessageSource* source) = 0;
	
	/**
	 * Alerts the listener that the source has closed.
	 *
	 * @param source	the source that has closed
	 */
	virtual void sourceClosed(class MessageSource* source) = 0;
};

#endif // MESSAGELISTENER_HPP
