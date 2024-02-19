#pragma once
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

private:
	unsigned __int64 playerBoard;
	unsigned __int64 robotBoard;
};

