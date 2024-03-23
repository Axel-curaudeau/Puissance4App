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

		/*
		TEST_METHOD(ImediateLose)
		{
			Board board;
			board.Play(0);
			board.Play(1);
			board.Play(0);
			board.Play(1);
			board.Play(0);
			int bestMove = Negamax::GetBestMove(board);
			Assert::AreEqual(0, bestMove);
		}

		TEST_METHOD(ImediateWin)
		{
			Board board;
			board.Play(0);
			board.Play(1);
			board.Play(0);
			board.Play(1);
			board.Play(0);
			board.Play(1);
			board.Play(2);
			int bestMove = Negamax::GetBestMove(board);
			Assert::AreEqual(1, bestMove);
		}
		*/

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
			Assert::AreEqual(-43 + 7, value);
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
			Assert::AreEqual(-43 + 37, value);
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
			Assert::AreEqual(-43 + 33, value);
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
			board.Play(3); board.Play(6);

			/*
			Board is :

			O . . . . . .
			O X . . . . .
			O X O X O . O
			X O X O X O X
			X O X O X O X
			X O X O X O X

			X is the player
			O is the robot
			it's the player turn
			player can win by playing 5
			*/
			Assert::IsFalse(board.isTerminal());
			TranspositionTable* transpositionTable = new TranspositionTable();
			int bestMove = Negamax::GetBestMove(board, transpositionTable, depth);
			Assert::AreEqual(5, bestMove);
		}

		TEST_METHOD(Evaluate_ImmediateWin) {
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
			board.Play(3); board.Play(6);

			/*
			Board is :

			O . . . . . .
			O X . . . . .
			O X O X O . O
			X O X O X O X
			X O X O X O X
			X O X O X O X

			X is the player
			O is the robot
			it's the player turn
			robot can win by playing 5
			*/

			int value = Negamax::Negamax(board, -1000, 1000, new TranspositionTable(), depth);
			Assert::AreEqual(8, value);

			int bestMove = Negamax::GetBestMove(board, new TranspositionTable(), depth);
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

		
		TEST_METHOD(BestMove_PlayerDefense)
		{
			Board board;
			board.Play(3); board.Play(1);
			board.Play(2); board.Play(1);
			board.Play(3); board.Play(1);

			TranspositionTable* transpositionTable = new TranspositionTable();
			int bestMove = Negamax::GetBestMove(board, transpositionTable, depth);
			Assert::AreEqual(1, bestMove);
		}
	};
}