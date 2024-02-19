#pragma once

#include <thread>
#include <iostream>
#include "Board.hpp"

namespace Negamax
{
	int GetBestMove(Board board);
	int Evaluate(Board board);
	int Negamax(Board board, int depth = 0);
	void NegamaxThread(Board board, int* result);
}


