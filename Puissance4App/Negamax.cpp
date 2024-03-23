#include "Negamax.hpp"

int Negamax::GetBestMove(Board board, TranspositionTable* transpositionTable, unsigned int depth)
{
	
	std::thread columnThreads[7];
	int* results = new int[7];

	for (int i = 0; i < 7; i++)
	{
		results[i] = -1000;
	}

	for (int i = 0; i < 7; i++)
	{
		if (board.isValidMove(i))
		{
			Board newBoard = board.copy();
			newBoard.Play(i);
			columnThreads[i] = std::thread(NegamaxThread, newBoard, &results[i], transpositionTable, depth);
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

	std::cout << "Results: ";
	for (int i = 0; i < 7; i++)
	{
		std::cout << results[i] << " ";
	}
	std::cout << std::endl;

	int bestMove = 0;

	int bestValue = -1000;
	for (int i = 0; i < 7; i++)
	{
		if (results[i] > bestValue)
		{
			bestValue = results[i];
			bestMove = i;
		}
	}
	return bestMove;
}

int Negamax::Evaluate(Board terminalBoard)
{
	
	if (terminalBoard.playerWins())
	{
		return -43 + terminalBoard.getMoveNumber();
	}
	else if (terminalBoard.robotWins())
	{
		return 43 - terminalBoard.getMoveNumber();
	}
	else
	{
		return 0;
	}
}

int Negamax::Negamax(Board board, int alpha, int beta, TranspositionTable* transpositionTable, unsigned int depth)
{
	/*
	if (transpositionTable->contains(board))
	{
		return transpositionTable->get(board);
	}
	*/
	if (depth == 0 || board.isTerminal())
	{
		int value = Negamax::Evaluate(board);
		return value;
	}
	else
	{
		int value = -1000;
		for (int i = 0; i < 7; i++)
		{
			if (board.isValidMove(i))
			{
				Board boardCopy = board.copy();
				boardCopy.Play(i);
				value = std::max(value, -Negamax::Negamax(boardCopy, -beta, -alpha, transpositionTable, depth - 1));
				alpha = std::max(alpha, value);
				if (alpha >= beta)
				{
					continue;
				}
			}
		}
		/*
		if (board.isTerminal())
		{
			transpositionTable->put(board, value);
		}*/
		
		return value;
	}
}

void Negamax::NegamaxThread(Board board, int* result, TranspositionTable* transpositionTable, unsigned int depth)
{
	*result = Negamax::Negamax(board, -100000, 100000, transpositionTable, depth);
}
