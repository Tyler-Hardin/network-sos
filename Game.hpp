/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * Header for game logic.
 */
 
#ifndef GAME_HPP
#define GAME_HPP

#include "SosBoard.hpp"

#include <list>
#include <map>
#include <string>

class Session;
class SosBoard;

/**
 * Represents a game of SOS. Stores player scores and board state.
 */
class Game {
	/** associative array mapping player name to score */
	std::map<Session*, int> scores;
	
	/** iterator to the player who's turn it is */
	std::map<Session*, int>::iterator _turn;
	
	/** game board */
	SosBoard board;
	
	/** track when a game must end */
	int moves_left;
	
	/** size of the board */
	int size;
	
public:
	/**
	 * Create a game and board.
	 *
	 * @param size		the size of the board
	 */
	Game(int size) : scores(), board(size), size(size), moves_left(0) {}
	
	/**
	 * Returns a copy of the board.
	 *
	 * @returns a copy of the board
	 */
	SosBoard getBoard();
	
	/**
	 * Resets the board and current player, effectively starting a new game.
	 */
	void start();
	
	/**
	 * Sets moves_left to 0 so no more will be accepted.
	 */
	void end();
	
	/**
	 * Check if the game is in a playable state.
	 *
	 * @returns if game is playing
	 */
	bool isOnGoing();
	
	/**
	 * Get the number of players.
	 *
	 * @returns number of players.
	 */
	int numPlayers();
	
	/**
	 * Adds a player to the game and initializes their score to 0.
	 *
	 * @param session	new player's session
	 */
	void addPlayer(Session* session);
	
	/**
	 * Removes a player from the game.
	 *
	 * @param session	player's name
	 */
	void removePlayer(Session* session);
	
	/**
	 * Gets the player who's turn it is.
	 *
	 * @returns the player who's turn it is
	 */
	Session* getPlayer();
	
	/**
	 * Gets a list of players and scores.
	 *
	 * @returns an associative array mapping player names to scores.
	 */
	std::map<Session*, int> getScores();
	
	/**
	 * Executes a move and returns the points scored. Also adds points to score of player.
	 * -1 is used to indicate an invalid move.
	 * 
	 * @param session	session of player moving
	 * @param c			'O' or 'S', whichever the player wants for the move
	 * @param row		the row for the move
	 * @param col		the column for the move
	 * @return 			the points scored
	 */
	int move(Session* session, char c, int row, int col);
};

#endif // GAME_HPP
