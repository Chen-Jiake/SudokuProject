#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\SudokuProject\sudoku.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		char ch[162] = {
			'3', ' ', '1', ' ', '2', ' ', '4', ' ', '5', ' ', '6', ' ', '7', ' ', '8', ' ', '9', '\n',
			'7', ' ', '8', ' ', '9', ' ', '3', ' ', '1', ' ', '2', ' ', '4', ' ', '5', ' ', '6', '\n',
			'4', ' ', '5', ' ', '6', ' ', '7', ' ', '8', ' ', '9', ' ', '3', ' ', '1', ' ', '2', '\n',
			'9', ' ', '3', ' ', '1', ' ', '2', ' ', '4', ' ', '5', ' ', '6', ' ', '7', ' ', '8', '\n',
			'6', ' ', '7', ' ', '8', ' ', '9', ' ', '3', ' ', '1', ' ', '2', ' ', '4', ' ', '5', '\n',
			'2', ' ', '4', ' ', '5', ' ', '6', ' ', '7', ' ', '8', ' ', '9', ' ', '3', ' ', '1', '\n',
			'8', ' ', '9', ' ', '3', ' ', '1', ' ', '2', ' ', '4', ' ', '5', ' ', '6', ' ', '7', '\n',
			'5', ' ', '6', ' ', '7', ' ', '8', ' ', '9', ' ', '3', ' ', '1', ' ', '2', ' ', '4', '\n',
			'1', ' ', '2', ' ', '4', ' ', '5', ' ', '6', ' ', '7', ' ', '8', ' ', '9', ' ', '3', '\0' };

		int expect_all_line[9][9] = {
			3, 1, 2, 4, 5, 6, 7, 8, 9,
			7, 8, 9, 3, 1, 2, 4, 5, 6,
			4, 5, 6, 7, 8, 9, 3, 1, 2,
			9, 3, 1, 2, 4, 5, 6, 7, 8,
			6, 7, 8, 9, 3, 1, 2, 4, 5,
			2, 4, 5, 6, 7, 8, 9, 3, 1,
			8, 9, 3, 1, 2, 4, 5, 6, 7,
			5, 6, 7, 8, 9, 3, 1, 2, 4,
			1, 2, 4, 5, 6, 7, 8, 9, 3
		};

		int expect_puzzle[9][9] = {
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 3, 0, 8, 5,
			0, 0, 1, 0, 2, 0, 0, 0, 0,
			0, 0, 0, 5, 0, 7, 0, 0, 0,
			0, 0, 4, 0, 0, 0, 1, 0, 0,
			0, 9, 0, 0, 0, 0, 0, 0, 0,
			5, 0, 0, 0, 0, 0, 0, 7, 3,
			0, 0, 2, 0, 1, 0, 0, 0, 0,
			0, 0, 0, 0, 4, 0, 0, 0, 9,
		};

		TEST_METHOD(TestGenerateMoveLine)
		{
			GenerateSudoku gen(1);
			int line[9] = { 3, 1, 2, 4, 5, 6, 7, 8, 9 };
			int** all_line = gen.move_line(line);
			bool f = true;
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					if (all_line[i][j] != expect_all_line[i][j])
						f = false;
				}
			}
			Assert::AreEqual(f, true);
		}

		TEST_METHOD(TestGenerateChIndex)
		{
			GenerateSudoku gen(1);
			gen.generate_sudoku();
			Assert::AreEqual(gen.ch_index, 161);
		}

		TEST_METHOD(TestGenerateAddIntoCh)
		{
			GenerateSudoku gen(1);
			int line[9] = { 3, 1, 2, 4, 5, 6, 7, 8, 9 };
			int** all_line = gen.move_line(line);
			int a[2] = { 1, 2 };
			int b[3] = { 3,4,5 };
			int c[3] = { 6,7,8 };
			gen.add_into_ch(all_line, a, b, c);
			gen.ch[gen.ch_index] = '\0';
			Assert::AreEqual(strcmp(ch, gen.ch), 0);
		}

		TEST_METHOD(TestGenetateSudoku)
		{
			GenerateSudoku gen(1);
			gen.generate_sudoku();
			gen.ch[gen.ch_index] = '\0';
			Assert::AreEqual(strcmp(ch, gen.ch), 0);
		}

		TEST_METHOD(TestSolveInvalidFilepath)
		{
			SolveSudoku slv("invalid_path.txt");
			Assert::AreEqual(slv.puzzle_file == NULL, true);
		}

		TEST_METHOD(TestWriteIntoFile)
		{
			bool f = true;
			GenerateSudoku gen(1);
			gen.ch[gen.ch_index++] = 't';
			gen.ch[gen.ch_index++] = 'e';
			gen.ch[gen.ch_index++] = 's';
			gen.ch[gen.ch_index++] = 't';
			gen.write_into_file();
			ifstream output("sudoku.txt");
			if (!output.is_open()) f = false;
			std::string str((std::istreambuf_iterator<char>(output)), std::istreambuf_iterator<char>());
			if (strcmp(str.data(), "test") != 0) f = false;
			output.close();
			Assert::AreEqual(f, true);
		}

		TEST_METHOD(TestSolveValidFilepath)
		{
			SolveSudoku slv("../UnitTest1/test_puzzle.txt");
			Assert::AreEqual(slv.puzzle_file != NULL, true);
		}

		TEST_METHOD(TestInputPuzzle)
		{
			SolveSudoku slv("../UnitTest1/test_input.txt");
			slv.input_puzzle();
			bool f = true;
			for (int i = 1; i <= 9; i++)
				if (slv.puzzle[0][i - 1] != i) f = false;
			Assert::AreEqual(f, true);
		}

		TEST_METHOD(TestSolveInputAndSolvePuzzle)
		{
			SolveSudoku slv("../UnitTest1/test_puzzle.txt");
			slv.input_puzzle();
			bool row[9][9];		//row[i][j]判断第i行是否已使用j
			bool column[9][9];	//column[i][j]判断第i列是否已使用j
			bool grid[9][9];	//grid[i][j]判断第i个九宫格是否已使用
			memset(row, false, sizeof(row));
			memset(column, false, sizeof(column));
			memset(grid, false, sizeof(grid));
			bool f = true;
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					int x = slv.puzzle[i][j];
					if (x == 0) f = false;
					if (!row[i][x - 1] && !column[j][x - 1] && !grid[i / 3 * 3 + j / 3][x - 1])
					{
						row[i][x - 1] = true;
						column[j][x - 1] = true;
						grid[i / 3 * 3 + j / 3][x - 1] = true;
					}
					else f = false;
				}
			}
			Assert::AreEqual(f, true);
		}

		TEST_METHOD(TestSolveAddIntoCh)
		{
			SolveSudoku slv("../UnitTest1/test_puzzle.txt");
			memcpy(slv.puzzle, expect_all_line, sizeof(expect_all_line));
			slv.add_into_ch();
			slv.ch[slv.ch_index] = '\0';
			Assert::AreEqual(strcmp(ch, slv.ch), 0);
		}

	};
}