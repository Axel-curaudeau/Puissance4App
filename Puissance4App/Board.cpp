#include "Board.hpp"

Board::Board()
{
	playerBoard = 0;
	robotBoard = 0;
	moveNumber = 0;
}

Board Board::copy()
{
	//Not Tested
	Board newBoard;
	newBoard.playerBoard = playerBoard;
	newBoard.robotBoard = robotBoard;
	newBoard.moveNumber = moveNumber;
	return newBoard;
}

void Board::Play(int column)
{
	for (int row = 0; row < 6; row++)
	{
		if (!getPiece(column, row, playerBoard) && !getPiece(column, row, robotBoard))
		{
			if (getMoveNumber() % 2 == 0)
			{
				setPiece(column, row, &playerBoard, true);
			}
			else
			{
				setPiece(column, row, &robotBoard, true);
			}
			moveNumber++;
			return;
		}
	}
	throw std::exception("Invalid move");
}

bool Board::isTerminal()
{
	if (playerWins() || robotWins() || draw())
	{
		return true;
	}
	return false;
}

bool Board::playerWins()
{
	return checkWin(playerBoard);
}

bool Board::robotWins()
{
	return checkWin(robotBoard);
}

bool Board::draw()
{
	if (isFull() && !playerWins() && !robotWins())
	{
		return true;
	}
	return false;
}

bool Board::isFull()
{
	if (getMoveNumber() == 42)
	{
		return true;
	}
	return false;
}

bool Board::isValidMove(int column)
{
	if (column < 0 || column > 6)
	{
		return false;
	}
	if (getPiece(column, 5, playerBoard) || getPiece(column, 5, robotBoard))
	{
		return false;
	}
	return true;
}

int Board::getMoveNumber()
{
	return moveNumber;
}

void Board::printBoard()
{
	for (int row = 5; row >= 0; row--)
	{
		for (int column = 0; column < 7; column++)
		{
			if (getPiece(column, row, playerBoard))
			{
				std::cout << "X ";
			}
			else if (getPiece(column, row, robotBoard))
			{
				std::cout << "O ";
			}
			else
			{
				std::cout << ". ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	if (getMoveNumber() % 2 == 0)
		std::cout << "Player's turn (X)" << std::endl;
	else
		std::cout << "Robot's turn (O)" << std::endl;
}

void Board::setPlayerPiece(int column, int row, bool value)
{
	setPiece(column, row, &playerBoard, value);
}

void Board::setRobotPiece(int column, int row, bool value)
{
	setPiece(column, row, &robotBoard, value);
}

bool Board::isEmpty()
{
	if (playerBoard == 0 && robotBoard == 0)
	{
		return true;
	}
	return false;
}

unsigned __int64 Board::getPlayerBitboard()
{
	return playerBoard;
}

unsigned __int64 Board::getRobotBitboard()
{
	return robotBoard;
}

bool Board::getPiece(int column, int row, unsigned __int64 board)
{
	unsigned __int64 mask = 1;
	mask = mask << (column + row * 7);
	if (board & mask)
	{
		return true;
	}
	return false;
}

void Board::setPiece(int column, int row, unsigned __int64* board, bool value)
{
	if (value)
	{
		moveNumber++;
	}
	else
	{
		moveNumber--;
	}

	unsigned __int64 mask = 1;
	mask = mask << (column + row * 7);
	if (value)
	{
		*board = *board | mask;
	}
	else
	{
		mask = ~mask;
		*board = *board & mask;
	}
}

bool Board::checkWin(unsigned __int64 board)
{
	//Check for 4 in a row
	for (int column = 0; column < 7; column++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (getPiece(column, row, board) && getPiece(column, row + 1, board) && getPiece(column, row + 2, board) && getPiece(column, row + 3, board))
			{
				return true;
			}
		}
	}

	//Check for 4 in a column
	for (int column = 0; column < 4; column++)
	{
		for (int row = 0; row < 6; row++)
		{
			if (getPiece(column, row, board) && getPiece(column + 1, row, board) && getPiece(column + 2, row, board) && getPiece(column + 3, row, board))
			{
				return true;
			}
		}
	}

	//Check for 4 in a diagonal from bottom left to top right
	for (int column = 0; column < 4; column++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (getPiece(column, row, board) && getPiece(column + 1, row + 1, board) && getPiece(column + 2, row + 2, board) && getPiece(column + 3, row + 3, board))
			{
				return true;
			}
		}
	}

	//Check for 4 in a diagonal from top left to bottom right
	for (int column = 0; column < 4; column++)
	{
		for (int row = 3; row < 6; row++)
		{
			if (getPiece(column, row, board) && getPiece(column + 1, row - 1, board) && getPiece(column + 2, row - 2, board) && getPiece(column + 3, row - 3, board))
			{
				return true;
			}
		}
	}
	return false;
}

/// <summary>
/// This fonction doses not work for all cases, it is a fast check for win
/// </summary>
bool Board::checkWinFast(unsigned __int64 board)
{
	unsigned __int64 m = board & (board >> 7);
	if (m & (m >> 14))
	{
		return true;
	}

	m = board & (board >> 6);
	if (m & (m >> 12))
	{
		return true;
	}

	m = board & (board >> 8);
	if (m & (m >> 16))
	{
		return true;
	}

	m = board & (board >> 1);
	if (m & (m >> 2))
	{
		return true;
	}

	return false;
}
