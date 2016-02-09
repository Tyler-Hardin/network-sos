/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * SOS Board implementation.
 */

#include "SosBoard.hpp"

#include <cstring>
#include <iostream>

inline char& SosBoard::board(int row, int col){
	// NOTE!!! This is returning a reference. (I.e. not just an lval).
	return _board[row * size + col];
}

std::string SosBoard::getAdjString(int row1, int col1, int row2, int col2){
	std::string ret;
	// Get the 3 adjacent chars.
	ret += board(row1, col1);
	ret += board((row1 + row2) / 2, (col1 + col2) / 2);
	ret += board(row2, col2);
	return ret;
}

SosBoard::SosBoard(int size) : 
	_board(new char[size * size]), 
	size(size){
	reset();
}

SosBoard::~SosBoard(){
//	if(_board != nullptr)
//		delete _board;
}

std::string SosBoard::toString(){
	std::string s;
	for(int row = 0;row < size + 2;row++){
		for(int col = 0;col < size + 2;col++){
			if(row == 0 || row == size + 1)
				s += '-';
			else if(col == 0 || col == size + 1)
				s += '|';
			else
				s += board(row - 1, col - 1);
		}
		s += '\n';
	}
	return s;
}

void SosBoard::reset(){
	// Initialize board to spaces.
	std::memset(_board, ' ', size * size);
}

int SosBoard::move(char c, int row, int col){
	int points = 0;
	
	// Don't do anything if (row, col) is already taken. Return -1 to indicate invalid move.
	if(board(row, col) == ' '){
		board(row, col) = c;
		
		if(c == 'O'){
			// Check all point cases for 'O' and add a point if necessary.
			
			// straight cases
			if(row > 0 && row + 1 < size && 
				getAdjString(row - 1, col, row + 1, col) == "SOS")
				points++;
			if(col > 0 && col + 1 < size && 
				getAdjString(row, col - 1, row, col + 1) == "SOS")
				points++;
			
			// diagonal cases
			if(row > 0 && row + 1 < size && col > 0 && col + 1 < size){
				if(getAdjString(row - 1, col - 1, row + 1, col + 1) == "SOS")
					points++;
				if(getAdjString(row - 1, col + 1, row + 1, col - 1) == "SOS")
					points++;
			}
		}
		else if(c == 'S'){
			// Check all point cases for 'S' and add a point if necessary.
			
			// straight cases
			if(row - 2 >= 0 && getAdjString(row - 2, col, row, col) == "SOS")
				points++;
			if(row + 2 < size && getAdjString(row, col, row + 2, col) == "SOS")
				points++;
			if(col - 2 >= 0 && getAdjString(row, col - 2, row, col) == "SOS")
				points++;
			if(col + 2 < size && getAdjString(row, col, row, col + 2) == "SOS")
				points++;
			
			// diagonal cases
			if(row - 2 >= 0 && col - 2 >= 0 && 
				getAdjString(row, col, row - 2, col - 2) == "SOS")
				points++;
			if(row - 2 >= 0 && col + 2 < size && 
				getAdjString(row, col, row - 2, col + 2) == "SOS")
				points++;
			if(row + 2 < size && col - 2 >= 0 && 
				getAdjString(row, col, row + 2, col - 2) == "SOS")
				points++;
			if(row + 2 < size && col + 2 < size && 
				getAdjString(row, col, row + 2, col + 2) == "SOS")
				points++;
		}
	}
	else
		points = -1;
	return points;
}
