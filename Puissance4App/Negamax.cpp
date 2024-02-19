#include "Negamax.hpp"

int Negamax::GetBestMove(Board board)
{
	std::thread columnThreads[7];
	int* results = new int[7];
	for (int i = 0; i < 7; i++)
	{
		if (board.isValidMove(i))
		{
			Board newBoard = board.copy();
			newBoard.Play(i);
			columnThreads[i] = std::thread(NegamaxThread, newBoard, &results[i]);
		}
		else
		{
			columnThreads[i] = std::thread();
		}
	}
	for (int i = 0; i < 7; i++)
	{
		if (columnThreads[i].joinable())
		{
			columnThreads[i].join();
		}
	}
	for (int i = 0; i < 7; i++)
	{
		std::cout << results[i] << std::endl;
	}
}

int Negamax::Evaluate(Board board)
{
	if (board.playerWins())
	{
		return -42 + board.getMoveNumber();
	}
	else if (board.robotWins())
	{
		return 42 - board.getMoveNumber();
	}
	else if (board.draw())
	{
		return 0;
	}
	else
	{
		throw new std::exception("Board is not terminal or invalid move");
		return 0;
	}
}

int Negamax::Negamax(Board board, int depth)
{
	if (board.isTerminal())
	{
		return Negamax::Evaluate(board);
	}
	else
	{
		int bestValue = -1000;
		for (int i = 0; i < 7; i++)
		{
			if (board.isValidMove(i))
			{
				Board boardCopy = board.copy();
				boardCopy.Play(i);
				int value = Negamax::Negamax(boardCopy, depth + 1);
				if (value > bestValue)
				{
					bestValue = value;
				}
			}
		}
		return bestValue;
	}
}

void Negamax::NegamaxThread(Board board, int* result)
{
	*result = Negamax::Negamax(board);
}
