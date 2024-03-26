#include "pch.h"
#include "CppUnitTest.h"
#include "../Puissance4App/Board.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Puissance4AppTests
{
	TEST_CLASS(BoardTest)
	{
	public:
		
		TEST_METHOD(EmptyBoard)
		{
			Board board;
			Assert::IsTrue(board.isEmpty());
			Assert::IsFalse(board.isFull());
			Assert::IsFalse(board.isTerminal());
		}

		TEST_METHOD(Board_copy)
		{
			Board board;
			board.Play(0);
			Board newBoard = board.copy();
			Assert::AreEqual(board.getMoveNumber(), newBoard.getMoveNumber());
			Assert::AreEqual(board.isEmpty(), newBoard.isEmpty());
			Assert::AreEqual(board.isFull(), newBoard.isFull());
			Assert::AreEqual(board.isTerminal(), newBoard.isTerminal());
			Assert::AreEqual(board.playerWins(), newBoard.playerWins());
			Assert::AreEqual(board.robotWins(), newBoard.robotWins());
		}

		TEST_METHOD(IsTerminal_NotTerminalBoard)
		{
			Board board;
			Assert::IsFalse(board.isTerminal());
		}

		TEST_METHOD(IsTerminal_TerminalBoard)
		{
			Board board;
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(0);
			Assert::IsTrue(board.isTerminal());
		}

		TEST_METHOD(BoardWithOnePiece)
		{
			Board board;
			board.Play(0);
			Assert::IsFalse(board.isEmpty());
			Assert::IsFalse(board.isFull());
			Assert::IsFalse(board.isTerminal());
		}

		TEST_METHOD(PlayerWiningBoard)
		{
			Board board;
			board.Play(0);board.Play(1);
			board.Play(0);board.Play(1);
			board.Play(0);board.Play(1);
			board.Play(0);
			Assert::IsFalse(board.isEmpty());
			Assert::IsTrue(board.isTerminal());
			Assert::IsTrue(board.playerWins());
			Assert::IsFalse(board.robotWins());
		}

		TEST_METHOD(RobotWiningBoard)
		{
			Board board;
			board.Play(0);
			board.Play(1);
			board.Play(0);
			board.Play(1);
			board.Play(0);
			board.Play(1);
			board.Play(2);
			board.Play(1);
			Assert::IsFalse(board.isEmpty());
			Assert::IsTrue(board.isTerminal());
			Assert::IsFalse(board.playerWins());
			Assert::IsTrue(board.robotWins());
		}

		

		TEST_METHOD(Board_CheckWinFast_PlayerWinVertical)
		{
			Board board;
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1); 
			board.Play(0); board.Play(1);
			board.Play(0);
			Assert::IsTrue(board.checkWinFast(board.getPlayerBitboard()));
			Assert::IsFalse(board.checkWinFast(board.getRobotBitboard()));
		}

		TEST_METHOD(Board_CheckWinFast_RobotWinVertical)
		{
			Board board;
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(2); board.Play(1);
			Assert::IsTrue(board.checkWinFast(board.getRobotBitboard()));
			Assert::IsFalse(board.checkWinFast(board.getPlayerBitboard()));
		}

		TEST_METHOD(Board_CheckWinFast_PlayerWinHorizontal)
		{
			Board board;
			board.Play(0); board.Play(0);
			board.Play(1); board.Play(1);
			board.Play(2); board.Play(2);
			board.Play(3);
			Assert::IsTrue(board.checkWinFast(board.getPlayerBitboard()));
			Assert::IsFalse(board.checkWinFast(board.getRobotBitboard()));
		}

		TEST_METHOD(Board_CheckWinFast_RobotWinHorizontal)
		{
			Board board;
			board.Play(0); board.Play(1);
			board.Play(1); board.Play(2);
			board.Play(2); board.Play(3);
			board.Play(3); board.Play(4);
			Assert::IsTrue(board.checkWinFast(board.getRobotBitboard()));
			Assert::IsFalse(board.checkWinFast(board.getPlayerBitboard()));
		}

		TEST_METHOD(Board_CheckWinFast_PlayerWinDiagonalRight)
		{
			Board board;
			board.Play(0); board.Play(1);
			board.Play(1); board.Play(2);
			board.Play(2); board.Play(3);
			board.Play(2); board.Play(3);
			board.Play(4); board.Play(3);
			board.Play(3);
			Assert::IsTrue(board.checkWinFast(board.getPlayerBitboard()));
			Assert::IsFalse(board.checkWinFast(board.getRobotBitboard()));
		}

		TEST_METHOD(Board_CheckWinFast_RobotWinDiagonalRight)
		{
			Board board;
			board.Play(1); board.Play(0);
			board.Play(2); board.Play(1);
			board.Play(2); board.Play(2);
			board.Play(3); board.Play(3);
			board.Play(3); board.Play(3);
			Assert::IsTrue(board.checkWinFast(board.getRobotBitboard()));
			Assert::IsFalse(board.checkWinFast(board.getPlayerBitboard()));
		}

		TEST_METHOD(Board_CheckWinFast_PlayerWinDiagonalLeft)
		{
			Board board;
			board.Play(6); board.Play(5);
			board.Play(5); board.Play(4);
			board.Play(4); board.Play(3);
			board.Play(4); board.Play(3);
			board.Play(2); board.Play(3);
			board.Play(3);
			Assert::IsTrue(board.checkWinFast(board.getPlayerBitboard()));
			Assert::IsFalse(board.checkWinFast(board.getRobotBitboard()));

		}

		TEST_METHOD(Board_CheckWinFast_RobotWinDiagonalLeft)
		{
			Board board;
			board.Play(5); board.Play(6);
			board.Play(4); board.Play(5);
			board.Play(4); board.Play(4);
			board.Play(3); board.Play(3);
			board.Play(3); board.Play(3);
			Assert::IsTrue(board.checkWinFast(board.getRobotBitboard()));
			Assert::IsFalse(board.checkWinFast(board.getPlayerBitboard()));
		}

		TEST_METHOD(Board_CheckWinFast_False)
		{
			Board board;
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(0); board.Play(1);
			board.Play(2); board.Play(3);
			Assert::IsFalse(board.checkWinFast(board.getRobotBitboard()));
			Assert::IsFalse(board.checkWinFast(board.getPlayerBitboard()));
		}
	};
}
