#include<cstdio>
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
	int all_line[9][9] = { 3, 9, 8, 7, 6, 5, 4, 2, 1 };
	int count = 0;
	int sum;
	clock_t start, end;
	ofstream generate_file;

	void move_line(int a[9], int b[9], int num)
	{
		for (int i = 0; i < 9; i++)
			a[(i + num) % 9] = b[i];
	}

	void write_into_file(int order[9])
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				generate_file << all_line[order[i]][j] << ' ';
			}
			generate_file << all_line[order[i]][8] << '\n';
		}
		for (int j = 0; j < 8; j++)
		{
			generate_file << all_line[order[8]][j] << ' ';
		}
		generate_file << all_line[order[8]][8];
		count++;
		if (count == sum)
		{
			generate_file.close();
			end = clock();
			double duration = (double)(end - start) / CLOCKS_PER_SEC;
			printf("%f seconds/n", duration);
			exit(0);
		}
		generate_file << '\n' << '\n';
	}

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
			int order[9] = { 0, 1, 2, 5, 4, 3, 8, 7, 6 };
			for (int i = 0; i < 6; i++)
			{
				next_permutation(order + 3, order + 6);
				for (int j = 0; j < 6; j++)
				{
					next_permutation(order + 6, order + 9);
					write_into_file(order);
				}				
			}
		}
	}

public:
	GenerateSudoku(int sum)
	{
		start = clock();
		count = 0;
		this->sum = sum;
		generate_file.open("sudoku.txt", ios::out);
		generate_sudoku();
	}
};

class SolveSudoku
{
private:
	int sum;
	int line_count = 0;
	int puzzle[9][9];
	int list[9] = { 1,2,3,4,5,6,7,8,9 };
	clock_t start, end;
	ofstream answer_file;
	ifstream puzzle_file;
	struct point
	{
		int x;
		int y;
		set<int> n_set;
		int n_sum;
	};

	void generate_points(point *p)
	{
		int start_i = (p->x / 3) * 3;
		int start_j = (p->y / 3) * 3;

		for (int i = start_i; i < start_i + 3; i++)
		{
			for (int j = start_j; j < start_j + 3; j++)
			{
				if ((*p).n_set.find(puzzle[i][j]) != (*p).n_set.end())
					(*p).n_set.erase(puzzle[i][j]);
			}
		}
		for (int i = 0; i < 9; i++)
		{
			if ((*p).n_set.find(puzzle[i][p->x]) != (*p).n_set.end())
				(*p).n_set.erase(puzzle[i][p->y]);
		}
		for (int j = 0; j < 9; j++)
		{
			if ((*p).n_set.find(puzzle[p->x][j]) != (*p).n_set.end())
				(*p).n_set.erase(puzzle[p->y][j]);
		}
		p->n_sum = (*p).n_set.size();
	}

	void write_into_file()
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				answer_file << puzzle[i][j] << ' ';
			}
			answer_file << puzzle[i][8] << '\n';
		}
		for (int j = 0; j < 8; j++)
		{
			answer_file << puzzle[8][j] << ' ';
		}
		answer_file << puzzle[8][8];
	}

	bool solve_puzzle(int i, int j)
	{
		while (puzzle[i][j] != 0)
		{
			if (j < 8) j = j + 1;
			else if (i < 8)
			{
				j = 0;
				i = i + 1;
			}
			else if (i == 8)
			{
				write_into_file();
				return true;
			}
		}
		point p;
		p.x = i;
		p.y = j;
		for (int n = 1; n <= 9; n++)
			p.n_set.insert(n);
		generate_points(&p);
		set<int>::iterator iter = p.n_set.begin();
		while (iter != p.n_set.end())
		{
			puzzle[i][j] = *iter;
			if (j < 8)
			{
				int r = solve_puzzle(i, j + 1);
				if (r) return true;
			}
			else if (i < 8)
			{
				int	r = solve_puzzle(i + 1, 0);
				if (r) return true;
			}
			else if (i == 8)
			{
				write_into_file();
				return true;
			}
			iter++;
		}
		puzzle[i][j] = 0;
		return false;
	}

	void input_puzzle()
	{
		bool first = true;
		while (!puzzle_file.eof())
		{
			if(!first) answer_file << '\n' << '\n';
			if (line_count != 0) answer_file << endl;
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					puzzle_file >> puzzle[i][j];
				}
			}
			solve_puzzle(0, 0);
			first = false;
		}
		puzzle_file.close();
		end = clock();
		double duration = (double)(end - start) / CLOCKS_PER_SEC;
		printf("%f seconds/n", duration);
	}

public:
	SolveSudoku(string puzzle_file_path)
	{
		start = clock();
		puzzle_file.open(puzzle_file_path, ios::in);
		if (!puzzle_file)
		{
			printf("请输入正确的文件\n");
			exit(0);
		}
		answer_file.open("sudoku.txt", ios::out);
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
