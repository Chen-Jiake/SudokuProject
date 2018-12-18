#include<cstdio>
#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>
#include<algorithm>
#include<ctime>
#include<set>
#include<vector>
using namespace std;

class GenerateSudoku
{
private:
	int all_line[9][9] = { 3, 9, 8, 7, 6, 5, 4, 2, 1 }; //存储数独终局
	int count = 0;    //行的数量
	int sum;          //需要生成的行的总数
	char ch[100100];  //存储生成的数独终局
	int ch_index = 0; //数组的索引
					  //（2，3）行的交换
	int order1[2][2] = { 1, 2,
		2, 1 };
	//（4，5，6）行的交换
	int order2[6][3] = { 3, 4, 5,
		3, 5, 4,
		4, 3, 5,
		4, 5, 3,
		5, 3, 4,
		5, 4, 3 };
	//（7，8，9）行的交换
	int order3[6][3] = { 6, 7, 8,
		6, 8, 7,
		7, 6, 8,
		7, 8, 6,
		8, 6, 7,
		8, 7, 6 };

	//向右移动
	void move_line(int a[9], int b[9], int num)
	{
		for (int i = 0; i < 9; i++)
			a[(i + num) % 9] = b[i];
	}

	//写文件
	void write_into_file(int order1[2], int order2[3], int order3[3])
	{
		for (int j = 0; j < 8; j++)
		{
			ch[ch_index++] = all_line[0][j] + '0';
			ch[ch_index++] = ' ';
		}
		ch[ch_index++] = all_line[0][8] + '0';
		ch[ch_index++] = '\n';
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				ch[ch_index++] = all_line[order1[i]][j] + '0';
				ch[ch_index++] = ' ';
			}
			ch[ch_index++] = all_line[order1[i]][8] + '0';
			ch[ch_index++] = '\n';
		}
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				ch[ch_index++] = all_line[order2[i]][j] + '0';
				ch[ch_index++] = ' ';
			}
			ch[ch_index++] = all_line[order2[i]][8] + '0';
			ch[ch_index++] = '\n';
		}
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				ch[ch_index++] = all_line[order3[i]][j] + '0';
				ch[ch_index++] = ' ';
			}
			ch[ch_index++] = all_line[order3[i]][8] + '0';
			ch[ch_index++] = '\n';
		}
		for (int j = 0; j < 8; j++)
		{
			ch[ch_index++] = all_line[order3[2]][j] + '0';
			ch[ch_index++] = ' ';
		}
		ch[ch_index++] = all_line[order3[2]][8] + '0';

		if (ch_index > 100000)
		{
			ch[ch_index] = '\0';
			cout << ch;
			ch_index = 0;
		}
		count++;
		if (count == sum)
		{
			ch[ch_index] = '\0';
			cout << ch;
			ch_index = 0;
			fclose(stdout);
			exit(0);
		}
		ch[ch_index++] = '\n';
		ch[ch_index++] = '\n';
	}

	//生成数独
	void generate_sudoku()
	{
		while (1)
		{
			next_permutation(all_line[0] + 1, all_line[0] + 9);
			move_line(all_line[1], all_line[0], 3);
			move_line(all_line[2], all_line[0], 6);
			move_line(all_line[3], all_line[0], 1);
			move_line(all_line[4], all_line[0], 4);
			move_line(all_line[5], all_line[0], 7);
			move_line(all_line[6], all_line[0], 2);
			move_line(all_line[7], all_line[0], 5);
			move_line(all_line[8], all_line[0], 8);
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					for (int k = 0; k < 6; k++)
					{
						write_into_file(order1[i], order2[j], order3[k]);
						write_into_file(order1[i], order3[j], order2[k]);
					}
				}
			}
		}
	}

public:
	GenerateSudoku(int sum)//构造函数
	{
		count = 0;
		this->sum = sum;
		freopen("sudoku.txt", "w", stdout);
		generate_sudoku();
	}
};

class SolveSudoku
{
private:
	int puzzle[9][9];  //存储数独问题
	char ch[100100];   //存储生成的数独终局
	int ch_index;      //数组的索引
	bool row[9][9];    //row[i][j]判断第i行是否已使用j
	bool column[9][9]; //column[i][j]判断第i列是否已使用j
	bool grid[9][9];   //grid[i][j]判断第i个九宫格是否已使用j

	//写文件
	void write_into_file()
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				ch[ch_index++] = puzzle[i][j] + '0';
				ch[ch_index++] = ' ';
			}
			ch[ch_index++] = puzzle[i][8] + '0';
			ch[ch_index++] = '\n';
		}
		for (int j = 0; j < 8; j++)
		{
			ch[ch_index++] = puzzle[8][j] + '0';
			ch[ch_index++] = ' ';
		}
		ch[ch_index++] = puzzle[8][8] + '0';;
		if (ch_index > 100000)
		{
			ch[ch_index] = '\0';
			cout << ch;
			ch_index = 0;
		}
	}

	//求解数独
	bool solve_puzzle(int i, int j)
	{
		while (puzzle[i][j] != 0)
		{
			if (j < 8) j++;
			else if (i < 8)
			{
				j = 0;
				i++;
			}
			else if (i == 8)
			{
				write_into_file();
				return true;
			}
		}
		for (int x = 1; x <= 9; x++)
		{
			if (!row[i][x - 1] && !column[j][x - 1] && !grid[i / 3 * 3 + j / 3][x - 1])
			{
				puzzle[i][j] = x;
				row[i][x - 1] = column[j][x - 1] = grid[i / 3 * 3 + j / 3][x - 1] = true;
				if (j < 8)
				{
					int r = solve_puzzle(i, j + 1);
					if (r) return true;
					else row[i][x - 1] = column[j][x - 1] = grid[i / 3 * 3 + j / 3][x - 1] = false;
				}
				else if (i < 8)
				{
					int	r = solve_puzzle(i + 1, 0);
					if (r) return true;
					else row[i][x - 1] = column[j][x - 1] = grid[i / 3 * 3 + j / 3][x - 1] = false;
				}
				else if (i == 8)
				{
					write_into_file();
					return true;
				}
			}
		}
		puzzle[i][j] = 0;
		return false;
	}

	//读入数独问题
	void input_puzzle()
	{
		bool first = true;
		int temp;
		int i = 0, j = 0;
		ch_index = 0;
		while (scanf("%d", &temp) != EOF)
		{
			if (i == 0 && j == 0)
			{
				if (!first)
				{
					ch[ch_index++] = '\n';
					ch[ch_index++] = '\n';
				}
				memset(row, false, sizeof(row));
				memset(column, false, sizeof(column));
				memset(grid, false, sizeof(grid));
				first = false;
			}
			puzzle[i][j] = temp;
			if (temp)
			{
				row[i][temp - 1] = true;
				column[j][temp - 1] = true;
				grid[i / 3 * 3 + j / 3][temp - 1] = true;
			}
			if (j < 8) j++;
			else if (i < 8)
			{
				i++;
				j = 0;
			}
			else
			{
				solve_puzzle(0, 0);
				i = j = 0;
			}
		}
		ch[ch_index] = '\0';
		cout << ch;
		ch_index = 0;
		fclose(stdout);
		fclose(stdin);
	}

public:
	SolveSudoku(char* puzzle_file_path) //构造函数
	{
		if (!freopen(puzzle_file_path, "r", stdin))
		{
			printf("请输入正确的文件路径\n");
			exit(0);
		}
		freopen("sudoku.txt", "w", stdout);
		input_puzzle();
	}
};

int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		if (strcmp(argv[1], "-c") == 0)
		{
			int len = strlen(argv[2]);
			int gen_sum = 0;
			for (int i = 0; i < len; i++)
			{
				if (argv[2][i] > '0' || argv[2][i] < '9')
				{
					gen_sum = gen_sum * 10 + (argv[2][i] - '0');
				}
				else
				{
					printf("请输入数字\n");
					break;
				}
			}
			GenerateSudoku gen(gen_sum);
		}
		else if (strcmp(argv[1], "-s") == 0)
		{
			SolveSudoku slv(argv[2]);
		}
		else printf("请输入正确指令\n");
	}
	else printf("请输入正确指令\n");
}
