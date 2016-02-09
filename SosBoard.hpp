/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * Header for game board.
 */

#ifndef SOSBOARD_HPP
#define SOSBOARD_HPP

#include <string>

/**
 * Represents an SOS game board.
 */
class SosBoard {
private:
	/** array of chars to store the state of the board */
	char* _board;
	
	/** size of the board (`size` by `size`) */
	int size;

protected:
	/**
	 * Provides a helper function for accessing the board.
	 * 
	 * @param row		the row to access
	 * @param col		the col to access
	 * @return			a reference to the char at (row, col)
	 */
	inline char& board(int row, int col);
	
	/**
	 * Returns the string of 3 adjacent cells.
	 *
	 * @param row1		the row of the beginning cell
	 * @param col1		the col of the beginning cell
	 * @param row2		the row of the end cell
	 * @param col2		the col of the end cell
	 * @returns			the string of 3 adjacent cells starting at (row1, col1) and ending at 
	 * (row2, col2)
	 */
	std::string getAdjString(int row1, int col1, int row2, int col2);

public:
	/**
	 * Creates board of size `size` by `size`.
	 *
	 * @param size		size of board
	 */
	SosBoard(int size);
	
	/**
	 * Destructs board and frees memory.
	 */
	~SosBoard();
	
	/**
	 * Return a string representation of the board.
	 *
	 * @param a string representation of the board
	 */
	std::string toString();
	
	/**
	 * Clears the board.
	 */
	void reset();
	
	/**
	 * Executes a move and returns the points scored. -1 is used to indicate an invalid move.
	 * 
	 * @param c			'O' or 'S', whichever the player wants for the move
	 * @param row		the row for the move
	 * @param col		the column for the move
	 * @return 			the points scored
	 */
	int move(char c, int row, int col);
};

#endif // SOSBOARD_HPP
