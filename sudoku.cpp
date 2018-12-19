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
	int first_line[9] = { 3, 9, 8, 7, 6, 5, 4, 2, 1 }; //终局第一行
	int sum;          //需要生成的终局总数
	char *ch;		  //存储生成的数独终局
	int ch_index;     //数组的索引
	//（2，3）行的交换
	int order1[2][2] =
	{ 1, 2,
		2, 1 };
	//（4，5，6）行的交换
	int order2[6][3] =
	{ 3, 4, 5,
		3, 5, 4,
		4, 3, 5,
		4, 5, 3,
		5, 3, 4,
		5, 4, 3 };
	//（7，8，9）行的交换
	int order3[6][3] =
	{ 6, 7, 8,
		6, 8, 7,
		7, 6, 8,
		7, 8, 6,
		8, 6, 7,
		8, 7, 6 };
	//第一行向右移动的位数
	int move[9] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };

	//将第一行向右移动得到数独终局
	int** move_line(int b[9])
	{
		int **a = (int **)malloc(9 * sizeof(int *));
		for (int i = 0; i < 9; i++)
			a[i] = (int *)malloc(9 * sizeof(int));
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				a[i][(j + move[i]) % 9] = b[j];
			}
		}
		return a;
	}

	//将生成的数独终局加入到数组中
	void add_into_ch(int** all_line, int order1[2], int order2[3], int order3[3])
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
	}

public:
	//写入文件
	void write_into_file()
	{
		ch[ch_index] = '\0';
		cout << ch;
		fclose(stdout);
	}

	//生成数独
	void generate_sudoku()
	{
		int count = 0;        //已生成的终局数量
		bool first = true;    //判断是不是第一行
		while (1)
		{
			next_permutation(first_line + 1, first_line + 9);
			int** all_line = move_line(first_line);
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					for (int k = 0; k < 6; k++)
					{
						if (first) first = false;
						else
						{
							ch[ch_index++] = '\n'; ch[ch_index++] = '\n';
						}
						add_into_ch(all_line, order1[i], order2[j], order3[k]);
						count++;
						if (count == sum) return;
						ch[ch_index++] = '\n'; ch[ch_index++] = '\n';
						add_into_ch(all_line, order1[i], order3[j], order2[k]);
						count++;
						if (count == sum) return;
					}
				}
			}
		}
	}
	//构造函数
	GenerateSudoku(int sum)
	{
		ch = new char[163000010];
		this->sum = sum;
		ch_index = 0;
		freopen("sudoku.txt", "w", stdout);
	}
	//析构函数
	~GenerateSudoku()
	{
		delete[] ch;
 	}
};

class SolveSudoku
{
private:
	int puzzle[9][9];  //存储数独问题
	char *ch;   //存储生成的数独终局
	int ch_index;      //数组的索引
	bool row[9][9];    //row[i][j]判断第i行是否已使用j
	bool column[9][9]; //column[i][j]判断第i列是否已使用j
	bool grid[9][9];   //grid[i][j]判断第i个九宫格是否已使用j

	//将生成的数独终局加入到数组中
	void add_into_ch()
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
				add_into_ch();
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
					add_into_ch();
					return true;
				}
			}
		}
		puzzle[i][j] = 0;
		return false;
	}

public:
	//读入数独问题
	void input_puzzle()
	{
		bool first = true;  //判断是不是第一行
		int temp;			//临时存储输入的数字
		int i = 0, j = 0;   //puzzle数组的索引
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
	}

	//写入文件
	void write_into_file()
	{
		ch[ch_index] = '\0';
		cout << ch;
		fclose(stdout);
		fclose(stdin);
	}

	SolveSudoku(char* puzzle_file_path) //构造函数
	{
		ch = new char[163000010];
		if (!freopen(puzzle_file_path, "r", stdin))
		{
			printf("请输入正确的文件路径\n");
			return;
		}
		ch_index = 0;
		freopen("sudoku.txt", "w", stdout);
	}

	~SolveSudoku() //析构函数
	{
		delete[] ch;
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
					return 0;
				}
			}
			GenerateSudoku gen(gen_sum);
			gen.generate_sudoku();
			gen.write_into_file();
		}
		else if (strcmp(argv[1], "-s") == 0)
		{
			SolveSudoku slv(argv[2]);
			slv.input_puzzle();
			slv.write_into_file();
		}
		else
		{
			printf("请输入正确指令\n");
			return 0;
		}
	}
	else printf("请输入正确指令\n");
}
