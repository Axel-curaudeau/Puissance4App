#pragma once

#include <exception>
#include <iostream>

class Board
{
public:
	Board();
	Board copy();
	void Play(int column);
	bool isTerminal();
	bool playerWins();
	bool robotWins();
	bool draw();
	bool isFull();
	bool isValidMove(int column);
	int getMoveNumber();
	void printBoard();
	void setPlayerPiece(int column, int row, bool value);
	void setRobotPiece(int column, int row, bool value);
	bool isEmpty();

private:
	unsigned __int64 playerBoard;
	unsigned __int64 robotBoard;
	unsigned int moveNumber;
	bool getPiece(int column, int row, unsigned __int64 board);
	void setPiece(int column, int row, unsigned __int64* board, bool value);
	bool checkWin(unsigned __int64 board);
};

