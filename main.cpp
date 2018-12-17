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
	int all_line[9][9] = { 3, 9, 8, 7, 6, 5, 4, 2, 1 };
	int count = 0;
	int sum;
	char ch[10100];
	int ch_index = 0;
	int order1[2][2] = { 1, 2,
						 2, 1 };
	int order2[6][3] = { 3, 4, 5,
						 3, 5, 4,
						 4, 3, 5,
						 4, 5, 3,
						 5, 3, 4,
						 5, 4, 3 };
	int order3[6][3] = { 6, 7, 8,
						 6, 8, 7,
						 7, 6, 8,
						 7, 8, 6,
						 8, 6, 7,
						 8, 7, 6 };
	void move_line(int a[9], int b[9], int num)
	{
		for (int i = 0; i < 9; i++)
			a[(i + num) % 9] = b[i];
	}

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
		
		if (ch_index > 10000)
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
	GenerateSudoku(int sum)
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
	int sum;
	int puzzle[9][9];
	int list[9] = { 1,2,3,4,5,6,7,8,9 };
	char ch[10100];
	int ch_index = 0;
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
	vector<point> possible_num_vector;

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
		if (ch_index > 10000)
		{
			ch[ch_index] = '\0';
			cout << ch;
			ch_index = 0;
		}
	}

	void delete_impossible_num(int x, int y, set<int> *s)
	{
		int start_i = (x / 3) * 3;
		int start_j = (y / 3) * 3;
		for (int i = start_i; i < start_i + 3; i++)
		{
			for (int j = start_j; j < start_j + 3; j++)
			{
				if ((*s).find(puzzle[i][j]) != (*s).end())
					(*s).erase(puzzle[i][j]);
			}
		}
		for (int i = 0; i < 9; i++)
		{
			if ((*s).find(puzzle[i][y]) != (*s).end())
				(*s).erase(puzzle[i][y]);
		}
		for (int j = 0; j < 9; j++)
		{
			if ((*s).find(puzzle[x][j]) != (*s).end())
				(*s).erase(puzzle[x][j]);
		}
	}

	bool solve_puzzle(int index)
	{
		if (index == possible_num_vector.size())
		{
			write_into_file();
			return true;
		}
		int i = possible_num_vector[index].x;
		int j = possible_num_vector[index].y;		
		set<int> s = possible_num_vector[index].n_set;
		delete_impossible_num(i, j, &s);
		for (set<int>::iterator iter =s.begin(); iter != s.end(); iter++)
		{			
			puzzle[i][j] = *iter;
			bool r = solve_puzzle(index + 1);
			if (r) return true;
		}
		puzzle[i][j] = 0;
		return false;
	}

	void generate_possible_num_vector()
	{
		for (int x = 0; x < 9; x++)
		{
			for (int y = 0; y < 9; y++)
			{
				if (puzzle[x][y] != 0) continue;
				point p;
				p.x = x;
				p.y = y;
				for (int i = 1; i <= 9; i++)
					p.n_set.insert(i);
				delete_impossible_num(x, y, &p.n_set);				
				p.n_sum = p.n_set.size();
				possible_num_vector.push_back(p);
			}
		}
	}

	//升序
	static bool less_sort(const point &a, const point &b)
	{
		if(a.n_sum != b.n_sum) return (a.n_sum < b.n_sum);
		else if (a.x != b.x)return (a.x < b.x);
		else return (a.y < b.y);
	}

	void input_puzzle()
	{
		bool first = true;
		while (!puzzle_file.eof())
		{
			if (!first)
			{
				ch[ch_index++] = '\n';
				ch[ch_index++] = '\n';
			}
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					puzzle_file >> puzzle[i][j];
				}
			}
			generate_possible_num_vector();
			sort(possible_num_vector.begin(), possible_num_vector.end(), less_sort);
			solve_puzzle(0);
			first = false;
		}
		ch[ch_index] = '\0';
		cout << ch;
		ch_index = 0;
		fclose(stdout);
		puzzle_file.close();
	}

public:
	SolveSudoku(char* puzzle_file_path)
	{
		puzzle_file.open(puzzle_file_path, ios::in);
		if (!puzzle_file)
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
