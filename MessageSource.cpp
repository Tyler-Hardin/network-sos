/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * MessageSource implementation.
 */

#include "MessageSource.hpp"

#include <iostream>

void MessageSource::notifyReceipt(std::string message){
	// lock_guard automatically locks `m` when it is constructed and unlocks when it 
	// is destructed.
	std::lock_guard<std::mutex> lock(m);
	
	for(MessageListener* listener : messageListeners){
		// Don't catch. No exceptions should be thrown, so let any exception this is thrown 
		// be fatal (or propogate).
		listener->messageReceived(message, this);
	}
}

void MessageSource::closeMessageSource(){
	// lock_guard automatically locks `m` when it is constructed and unlocks when it 
	// is destructed.
	std::lock_guard<std::mutex> lock(m);
	
	// C++ doesn't have ConcurrentModificationException.
	for(MessageListener* listener : messageListeners){
		// Don't catch. No exceptions should be thrown, so let any exception this is thrown 
		// be fatal (or propogate).
		listener->sourceClosed(this);
	}
}

void MessageSource::addMessageListener(MessageListener* listener){
	// lock_guard automatically locks `m` when it is constructed and unlocks when it 
	// is destructed.
	std::lock_guard<std::mutex> lock(m);
	
	messageListeners.push_back(listener);
}

void MessageSource::removeMessageListener(MessageListener* listener){
	// lock_guard automatically locks `m` when it is constructed and unlocks when it 
	// is destructed.
	std::lock_guard<std::mutex> lock(m);
	
	messageListeners.remove(listener);
}
