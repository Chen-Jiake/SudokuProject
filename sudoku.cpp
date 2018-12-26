#include<cstdio>
#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>
#include<algorithm>
#include<ctime>
#include<vector>
using namespace std;

class GenerateSudoku
{
public:
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
		//第1行
		for (int j = 0; j < 8; j++)
		{
			ch[ch_index++] = all_line[0][j] + '0';
			ch[ch_index++] = ' ';
		}
		ch[ch_index++] = all_line[0][8] + '0';
		ch[ch_index++] = '\n';
		
		//第2-3行
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

		//第4-6行
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

		//第7-8行
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

		//第9行
		for (int j = 0; j < 8; j++)
		{
			ch[ch_index++] = all_line[order3[2]][j] + '0';
			ch[ch_index++] = ' ';
		}
		ch[ch_index++] = all_line[order3[2]][8] + '0';
	}

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
		while (true)
		{
			next_permutation(first_line + 1, first_line + 9); //全排列生成第一行的数字
			int** all_line = move_line(first_line);  //调用move_line函数，通过将第一行向右移动获得数独终局
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					for (int k = 0; k < 6; k++)
					{
						if (first) first = false;   
						else													//如果不是第一行，需要在字符串中增加两个回车（终局与终局之间的空行）
						{
							ch[ch_index++] = '\n'; ch[ch_index++] = '\n';
						}
						add_into_ch(all_line, order1[i], order2[j], order3[k]); //按照order的顺序将行加入字符串中
						count++;                                                //生成的终局数+1
						if (count == sum) return;                               //如果已达到生成数量要求就返回
						ch[ch_index++] = '\n'; ch[ch_index++] = '\n';           //终局与终局之间的空行
						add_into_ch(all_line, order1[i], order3[j], order2[k]); //按照order的顺序将行加入字符串中
						count++;                                                //生成的终局数+1
						if (count == sum) return;                               //如果已达到生成数量要求就返回
					}
				}
			}
		}
	}
	//构造函数
	GenerateSudoku(const int sum)
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
public:
	int puzzle[9][9];	//存储数独问题
	char *ch;			//存储生成的数独终局
	int ch_index;		//数组的索引
	bool row[9][9];		//row[i][j]判断第i行是否已使用j
	bool column[9][9];	//column[i][j]判断第i列是否已使用j
	bool grid[9][9];	//grid[i][j]判断第i个九宫格是否已使用j
	FILE *puzzle_file;	//输入文件

	//将生成的数独终局加入到数组中
	void add_into_ch()
	{
		//第1-8行
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

		//第9行
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
		//若当前位置的值不为0，就按行寻找直到当前位置的值为0
		while (puzzle[i][j] != 0)
		{
			if (j < 8) j++;
			else if (i < 8)
			{
				j = 0;
				i++;
			}
			else if (i == 8)  //遍历到最后一个位置不为0，说明已经成功生成解
			{
				add_into_ch();
				return true;
			}
		}
		for (int x = 1; x <= 9; x++)  //用1到9为当前位置赋值
		{
			if (!row[i][x - 1] && !column[j][x - 1] && !grid[i / 3 * 3 + j / 3][x - 1])  //符合数组的三条规则
			{
				puzzle[i][j] = x;      //为当前位置赋值
				row[i][x - 1] = column[j][x - 1] = grid[i / 3 * 3 + j / 3][x - 1] = true;//在行、列、九宫格的相应位置进行标记
				if (j < 8)
				{
					int r = solve_puzzle(i, j + 1);  //r为true，说明成功生成解
					if (r) return true;
					else row[i][x - 1] = column[j][x - 1] = grid[i / 3 * 3 + j / 3][x - 1] = false; //发生错误，重置为false
				}
				else if (i < 8)
				{
					int	r = solve_puzzle(i + 1, 0);
					if (r) return true;
					else row[i][x - 1] = column[j][x - 1] = grid[i / 3 * 3 + j / 3][x - 1] = false;
				}
				else if (i == 8)
				{
					add_into_ch();   //最后一个位置成功赋值，说明解成功
					return true;
				}
			}
		}
		puzzle[i][j] = 0; //1-9都不符合条件
		return false;     //解失败
	}

	//读入数独问题
	void input_puzzle()
	{
		bool first = true;   //判断是不是第一行
		char temp[20];		 //临时存储输入的数字
		int line= 0;         //输入的是第几行
		while (gets_s(temp)) 
		{
			if (strlen(temp) == 0) continue;  //跳过空行
			if (line == 0)                    //如果是第一行
			{
				if (!first)                   //解之间的空行
				{
					ch[ch_index++] = '\n';
					ch[ch_index++] = '\n';
				}
				//初始化三个二维数组
				memset(row, false, sizeof(row));          
				memset(column, false, sizeof(column));
				memset(grid, false, sizeof(grid));
				first = false;
			}
			for (int j = 0; j < 17; j++)
			{
				if (temp[j] != ' ')
				{
					int temp_num = temp[j] - '0';
					puzzle[line][j / 2] = temp_num;   //将输入的数字存入puzzle数组中
					if (temp_num)
					{
						row[line][temp_num - 1] = true;
						column[j / 2][temp_num - 1] = true;
						grid[line / 3 * 3 + j / 6][temp_num - 1] = true;
					}
				}
			}
			line++;
			if (line == 9)           //9行输入完成之后调用solve_puzzle求解问题
			{
				solve_puzzle(0, 0);
				line = 0;
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
		puzzle_file = freopen(puzzle_file_path, "r", stdin);
		if (!puzzle_file)
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
				if (argv[2][i] > '0' || argv[2][i] < '9')        //将数独终局的数量转变为数字
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