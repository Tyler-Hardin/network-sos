/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * MessageSource interface.
 */

#ifndef MESSAGESOURCE_HPP
#define MESSAGESOURCE_HPP

#include "MessageListener.hpp"

#include <list>
#include <mutex>
#include <string>

/**
 * Represents a source.
 */
class MessageSource {
	/** list of listeners */
	std::list<class MessageListener*> messageListeners;
	
	/** lock for listeners list */
	std::mutex m;
	
protected:
	/**
	 * Send a message to all the listeners of this source.
	 *
	 * @param message	the message to send
	 */
	void notifyReceipt(std::string message);

	/**
	 * Close this message source.
	 */
	void closeMessageSource();


public:
	/** 
	 * Default initialize instance vars.
	 */
	MessageSource() : messageListeners(), m() {}

	/**
	 * Adds a listener to this source. Precondition: listener must not be null.
	 *
	 * @param listener	listener to add
	 */
	void addMessageListener(MessageListener* listener);
	
	/**
	 * Removes a listener from this source.
	 *
	 * @param listener	listener to remove.
	 */
	void removeMessageListener(MessageListener* listener);
};

#endif // MESSAGESOURCE_HPP
