#include "pch.h"
#include "CppUnitTest.h"
#include "../Puissance4App/TranspositionTable.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Puissance4AppTests
{
	TEST_CLASS(TranspositionTableTest)
	{
	public:
		TEST_METHOD(TranspositionTable_ContainsBoard)
		{
			TranspositionTable table;
			Board board;
			board.Play(0);
			table.put(board, 42);
			Assert::IsTrue(table.contains(board));
		}

		TEST_METHOD(TranspositionTable_DoesNotContainBoard)
		{
			TranspositionTable table;
			Board board;
			board.Play(0);
			Assert::IsFalse(table.contains(board));
		}

		TEST_METHOD(TranspositionTablePutGet)
		{
			TranspositionTable table;
			Board board;
			board.Play(0); board.Play(1);
			table.put(board, 42);
			if (table.contains(board) == false)
			{
				Assert::Fail();
			}
			int value = table.get(board);
			Assert::AreEqual(42, value);
		}

		TEST_METHOD(TranspositionTablePutGet2)
		{
			TranspositionTable table;
			Board board;
			board.Play(0); board.Play(1);
			table.put(board, 42);
			board.Play(2);
			table.put(board, 41);
			if (table.contains(board) == false)
			{
				Assert::Fail();
			}
			int value = table.get(board);
			Assert::AreEqual(41, value);
		}
	};
}