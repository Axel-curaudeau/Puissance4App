#include "pch.h"
#include "CppUnitTest.h"
#include "../Puissance4App/Negamax.cpp"
#include "../Puissance4App/TranspositionTable.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Puissance4AppTests
{
	TEST_CLASS(NegamaxTest)
	{
	private:
		int depth = 8;
	public:

		TEST_METHOD(LowDepthNegamax_RobotWin)
		{
			Board board;
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(2); board.Play(3);
			board.Play(2); board.Play(3);
			board.Play(2); board.Play(3);
			board.Play(4); board.Play(5);
			board.Play(4); board.Play(5);
			board.Play(4); board.Play(5);
			board.Play(6); board.Play(0);
			board.Play(6); board.Play(0);
			board.Play(6); board.Play(0);
			board.Play(1); board.Play(2);
			board.Play(1); board.Play(4);
			board.Play(1); board.Play(6);
			board.Play(5);

			/*
			Board is :

			O X . . . . .
			O X . . . . .
			O X O . O X O
			X O X O X O X
			X O X O X O X
			X O X O X O X

			X is the player
			O is the robot
			it's the robot turn
			robot can win by playing 3
			*/

			TranspositionTable* transpositionTable = new TranspositionTable();
			int bestMove = Negamax::GetBestMove(board, transpositionTable, depth);
			Assert::AreEqual(3, bestMove);
		}
		

		TEST_METHOD(Evaluate_PlayerWinIn7Move)
		{
			Board board;
			board.Play(0);board.Play(1);
			board.Play(0);board.Play(1);
			board.Play(0);board.Play(1);
			board.Play(0);
			int value = Negamax::Evaluate(board);
			Assert::AreEqual(36, value);
		}

		TEST_METHOD(Evaluate_RobotWinIn8Move)
		{
			Board board;
			board.Play(0);board.Play(1);
			board.Play(0);board.Play(1);
			board.Play(0);board.Play(1);
			board.Play(2);board.Play(1);
			int value = Negamax::Evaluate(board);
			Assert::AreEqual(43 - 8, value);
		}

		TEST_METHOD(Evaluate_Draw)
		{
			Board board;
			board.Play(0);board.Play(1);
			board.Play(0);board.Play(1);
			board.Play(0);board.Play(1);
			board.Play(2);board.Play(3);
			board.Play(2);board.Play(3);
			board.Play(2);board.Play(3);
			board.Play(4);board.Play(5);
			board.Play(4);board.Play(5);
			board.Play(4);board.Play(5);
			board.Play(6);board.Play(0);
			board.Play(6);board.Play(0);
			board.Play(6);board.Play(0);
			board.Play(1);board.Play(2);
			board.Play(1);board.Play(2);
			board.Play(1);board.Play(2);
			board.Play(3);board.Play(4);
			board.Play(3);board.Play(4);
			board.Play(3);board.Play(4);
			board.Play(5);board.Play(6);
			board.Play(5);board.Play(6);
			board.Play(5);board.Play(6);

			/*
			board is :

			O X O X O X O
			O X O X O X O
			O X O X O X O
			X O X O X O X
			X O X O X O X
			X O X O X O X

			*/
			int value = Negamax::Evaluate(board);
			Assert::AreEqual(0, value);
		}

		TEST_METHOD(Evaluate_PlayerWinIn37Move)
		{
			Board board;
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(2); board.Play(3);
			board.Play(2); board.Play(3);
			board.Play(2); board.Play(3);
			board.Play(4); board.Play(5);
			board.Play(4); board.Play(5);
			board.Play(4); board.Play(5);
			board.Play(6); board.Play(0);
			board.Play(6); board.Play(0);
			board.Play(6); board.Play(0);
			board.Play(1); board.Play(2);
			board.Play(1); board.Play(2);
			board.Play(1); board.Play(2);
			board.Play(3); board.Play(4);
			board.Play(3); board.Play(4);
			board.Play(3); board.Play(4);
			board.Play(6);
			int value = Negamax::Evaluate(board);
			Assert::AreEqual(6, value);
		}

		TEST_METHOD(Negamax_Score_RobotVictoryIn32Moves)
		{
			Board board;
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(2); board.Play(3);
			board.Play(2); board.Play(3);
			board.Play(2); board.Play(3);
			board.Play(4); board.Play(5);
			board.Play(4); board.Play(5);
			board.Play(4); board.Play(5);
			board.Play(6); board.Play(0);
			board.Play(6); board.Play(0);
			board.Play(6); board.Play(0);
			board.Play(1); board.Play(2);
			board.Play(1); board.Play(2);
			board.Play(1); board.Play(2);
			board.Play(3);

			TranspositionTable* transpositionTable = new TranspositionTable();
			int value = Negamax::Negamax(board, -1000, 1000, transpositionTable, depth);
			Assert::AreEqual(12, value);
		}

		TEST_METHOD(LowDepthNegamax_PlayerWin)
		{
			Board board;
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(2); board.Play(3);
			board.Play(2); board.Play(3);
			board.Play(2); board.Play(3);
			board.Play(4); board.Play(5);
			board.Play(4); board.Play(5);
			board.Play(4); board.Play(5);
			board.Play(6); board.Play(0);
			board.Play(6); board.Play(0);
			board.Play(6); board.Play(0);
			board.Play(1); board.Play(2);
			board.Play(1); board.Play(4);
			board.Play(3);

			/*
			Board is :

			O . . . . . .
			O X . . . . .
			O X O X O . .
			X O X O X O X
			X O X O X O X
			X O X O X O X

			X is the player
			O is the robot
			it's the robot turn
			robot can win by playing 5
			*/
			Assert::IsFalse(board.isTerminal());
			TranspositionTable* transpositionTable = new TranspositionTable();
			int bestMove = Negamax::GetBestMove(board, transpositionTable, depth);
			Assert::AreEqual(5, bestMove);
		}

		TEST_METHOD(BestMove_RobotDefense)
		{
			Board board;
			board.Play(3); board.Play(1);
			board.Play(3); board.Play(1);
			board.Play(3);

			TranspositionTable* transpositionTable = new TranspositionTable();
			int bestMove = Negamax::GetBestMove(board, transpositionTable, depth);
			Assert::AreEqual(3, bestMove);
		}

		
		TEST_METHOD(BestMove_RobotWin)
		{
			Board board;
			board.Play(1); board.Play(3);
			board.Play(2); board.Play(3);
			board.Play(1); board.Play(3);
			board.Play(2);

			TranspositionTable* transpositionTable = new TranspositionTable();
			int bestMove = Negamax::GetBestMove(board, transpositionTable, depth);
			Assert::AreEqual(3, bestMove);
		}
	};
}