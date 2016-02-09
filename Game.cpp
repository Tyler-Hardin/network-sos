/**
 * Tyler Hardin
 * Networking Project 2
 * 12/01/14
 *
 * Implementation of game logic.
 */

#include "Game.hpp"
#include "Session.hpp"

SosBoard Game::getBoard(){
	return board;
}

void Game::start(){
	// Clear board.
	board.reset();
	
	// Reset scores to 0.
	for(auto& i : scores)
		i.second = 0;
	
	// Reset currect player.
	_turn = scores.begin();
	
	moves_left = size * size;
}

void Game::end(){
	moves_left = 0;
}

bool Game::isOnGoing(){
	return moves_left > 0;
}

int Game::numPlayers(){
	return scores.size();
}

void Game::addPlayer(Session* session){
	// insert only inserts if name doesn't exist
	scores.insert(make_pair(session, 0));
}

void Game::removePlayer(Session* session){
	scores.erase(session);
}

Session* Game::getPlayer(){
	Session* p = nullptr;
	
	// Check that iterator isn't undefined.
	if(_turn != scores.end())
		p = _turn->first;
	
	return p;
}

std::map<Session*, int> Game::getScores(){
	return scores;
}

int Game::move(Session* s, char c, int row, int col){
	int points = 0;
	
	// Normalize c.
	if(c == 'c')
		c = 'C';
	else if(c == 's')
		c = 'S';
	
	// Get a reference to the name/score pair of this player.
	auto score_pair = scores.find(s);
	
	// Make sure `name` is a player in the list.
	if(score_pair != scores.end()){
		// Get points for this move.
		points = board.move(c, row, col);
		
		if(points >= 0){
			// Add points to score.
			score_pair->second += points;
		}
	}
	
	// If it was a valid move, then subtract it and move to next player.
	if(points >= 0){
		moves_left--;
		
		_turn++;
		if(_turn == scores.end())
			_turn = scores.begin();
	}
	
	return points;
}
