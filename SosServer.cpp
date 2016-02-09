/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * Server implementation.
 */

#include "SosServer.hpp"

#include <boost/algorithm/string.hpp>

#include <iostream>

namespace ip = boost::asio::ip;
using boost::asio::ip::tcp;

void SosServer::game_over(){
	broadcast("Score");
	auto scores = game.getScores();
	for(auto i : scores)
		broadcast(i.first->getName() + ": " + std::to_string(i.second));
	broadcast("GAME OVER");
}

void SosServer::listen(){
	boost::asio::io_service io_service;
	boost::system::error_code ec;
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port), ec);
	
	while(!ec){
		// Create a session to handle the next incoming connection.
		std::shared_ptr<Session> session(new Session);
		
		// Accept a new connection on the new session.
		acceptor.accept(session->socket(), ec);
		
		if(!ec){
			// Set the server to listen for incoming data.
			session->addMessageListener(this);
			
			// Set the session to listen for broadcasts from the server.
			addMessageListener(session.get());
			
			// Create a new thread and call session->start()
			std::thread t(std::bind(&Session::start, session));
			
			// Store session and thread.
			sessions.push_back(session);
			threads.push_back(std::move(t));
		}
	}
	
	if(ec)
		throw boost::system::system_error(ec);
}

void SosServer::broadcast(std::string msg){
	notifyReceipt(msg);
}

void SosServer::messageReceived(std::string msg, class MessageSource* source){
	// Normalize string to lowercase.
	boost::to_lower(msg);
	
	// Erase newlines.
	boost::erase_all(msg, "\n");
	
	// Split the message by spaces.
	std::vector<std::string> substrs;
	boost::split(substrs, msg, boost::is_any_of(" "), boost::token_compress_on);
	
	// Remove extra entries (caused by extraneous spaces).
	auto i = substrs.begin();
	while(i < substrs.end())
		if(*i != "")
			i++;
		else	
			substrs.erase(i);
	
	//std::cout << msg << std::endl;
	
	// We know because all sources are listeners in this code base that source is a listener, so 
	// this is technically safe. I couldn't find a better way to do this, without changing the 
	// design to have a class that is both listener and source and getting rid of the separate ones.
	Session* session = (Session*)(source);
	
	if(substrs.size() == 0){ /* do nothing */ }
	else if(substrs[0] == "/connect"){
		// Check for errors.
		if(substrs.size() != 2)
			session->send("/connect [name]");
		else if(game.isOnGoing()){
			session->send("Game on going. Try again later.");
			// We could block them, but why? It won't break anything to let them stay connected
			// while they wait.
			// session->close();
		}
		else if(session->getName() != ""){
			broadcast(session->getName() + " changed nick to " + substrs[1]);
			session->setName(substrs[1]);
		}
		else{
			auto name = substrs[1];
			session->setName(name);
			broadcast("--> " + name + " has joined");
			
			// Add this session to the list of valid players.
			game.addPlayer(session);
		}
	}
	else if(substrs[0] == "/play"){
		// Check for error.
		if(game.numPlayers() <= 1)
			session->send("Not enough players to play the game");
		else if(game.isOnGoing())
			session->send("Play already in progress");
		else{
			// Initialize game state.
			game.start();
			
			// Print board and other stuff.
			broadcast(game.getBoard().toString());
			broadcast("The game begins");
			broadcast(game.getPlayer()->getName() + " it is your turn");
		}
	}
	else if(substrs[0] == "/move"){
		// Check for errors.
		if(!game.isOnGoing())
			session->send("Game has not started");
		else if(substrs.size() != 4)
			session->send("/move [oO|sS] [row] [col]");
		else if(session != game.getPlayer())
			broadcast("Invalid move: " + game.getPlayer()->getName() + " it is your turn");
		else{
			// Normalize string to simplify next error check.
			boost::to_upper(substrs[1]);
			
			// One last error, is the letter an O or S?
			if(substrs[1].at(0) != 'O' && substrs[1].at(0) != 'S')
				broadcast("Invalid move: " + game.getPlayer()->getName() + " it is your turn");
			else{
				// Get the points for the move. -1 if invalid.
				int points = game.move(session, substrs[1].at(0), stoi(substrs[2]), 
				stoi(substrs[3]));
				
				// Vaild move?
				if(points >= 0){
					broadcast(game.getBoard().toString());
					broadcast(session->getName() + " scored " + std::to_string(points) + " points");
					broadcast(game.getPlayer()->getName() + " it is your turn");
					
					// Last move?
					if(!game.isOnGoing())
						game_over();
				}
				else
					broadcast("Invalid move: " + game.getPlayer()->getName() + " it is your turn");
			}
		}
	}
	else if(substrs[0] == "/quit"){
		// Let the players know who killed their game.
		broadcast(session->getName() + " quit");
		
		if(game.isOnGoing()){
			// Print the score.
			game_over();
		}
		
		// End the current game.
		game.end();
		
		// Disconnect them.
		session->close();
	}
	else{ // Dont print error if empty command.
		session->send("Command \"" + substrs[0] + "\" unrecognized"); 
	}
}

void SosServer::sourceClosed(MessageSource* source){	
	auto session_it = sessions.begin();
	auto thread_it = threads.begin();
	
	// While it is seemingly not type-safe, this is actually okay, since pointers are unique and 
	// only MessageListeners are in the list. If it finds a pointer equal to source in the listener
	// list, we know it must be a MessageListener (assuming only MessageListeners are added to the 
	// list).
	removeMessageListener((MessageListener*)(Session*)source);
	game.removePlayer((Session*)source);
	
	// Find iterator that points to the closed session and its corresponding thread.
	while(session_it->get() != source){
		session_it++;
		thread_it++;
	}
	
	// Detach thread. (Actually, it will be the thread itself calling this, so we CAN'T join (the
	// thread would block waiting for itself to end). We must detach.
	thread_it->detach();
	
	// Erase the thread from the list.
	threads.erase(thread_it);
	
	// Erase the session smart pointer from the list.
	// Even though we're returning to the object who's smart pointer we just deleted, it won't be 
	// destructed until the method we return to exits (b/c we created another smart pointer to it
	// in the methods scope just to make sure it wasn't destructed. So we're not returning to a
	// *destructed* object.
	// It was hard to figure out a system that both removed the session/thread from the list but 
	// didn't involve returning to a destructed object.
	sessions.erase(session_it);
	
	std::cout << "Connection closed." << std::endl;
}
