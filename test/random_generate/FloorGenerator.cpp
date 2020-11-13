#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;

class RP
{
private:
	int *randPool;
	int pointer;
	int row;
	int col;

public:
	RP(int row_in = 0, int col_in = 0);
	void get_rand(int &, int &);
	void delNum(int);
};

RP::RP(int row_in, int col_in)
{
	randPool = new int[row_in * col_in];
	for (int i = 0; i < row_in * col_in; i++)
	{
		randPool[i] = i;
	}
	pointer = row_in * col_in - 1;
	row = row_in;
	col = col_in;
	for (int i = 0; i < col; i++)
	{
		delNum(i);
		delNum((row - 1) * col + i);
	}
	for (int i = 0; i < row; i++)
	{
		delNum(col * i);
		delNum(col * (i + 1) - 1);
	}
}

void RP::get_rand(int &a, int &b)
{
	int index_temp = rand() % (pointer + 1);
	//cout << "index_temp = " << index_temp << '\n';
	int result = randPool[index_temp];
	//cout << "result = " << result << '\n';
	delNum(result);
	a = result / col;
	b = result % col;
}

void RP::delNum(int n)
{
	int i = 0;
	bool flag = true;
	while (flag && i <= pointer)
	{
		if (randPool[i] == n)
		{
			flag = false;
			randPool[i] = randPool[pointer];
			pointer--;
		}
		i++;
	}
}

int main()
{
	int row = 15;		//自訂row
	int col = 15;		//自訂col
	int percent = 70; //自訂空地百分比(單位:%) 範例:70

	int clock_temp = clock();
	srand(time(0));

	int **floor;
	floor = new int *[row];
	for (int i = 0; i < row; i++)
	{
		floor[i] = new int[col];
	}

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			floor[i][j] = -1;
		}
	}

	int R_row = 0;
	int R_col = 0;

	while ((R_row == 0 && R_col == 0) || (R_row == 0 && R_col == col - 1) || (R_row == row - 1 && R_col == 0) || (R_row == row - 1 && R_col == col - 1))
	{
		R_row = rand() % row;
		R_col = rand() % col;
	}

	int cur_row = R_row;
	int cur_col = R_col;
	if (R_row == 0)
	{
		cur_row = 1;
	}
	if (R_row == row - 1)
	{
		cur_row = row - 2;
	}
	if (R_col == 0)
	{
		cur_col = 1;
	}
	if (R_col == col - 1)
	{
		cur_col = col - 2;
	}

	RP *rp = new RP(row, col);
	rp->delNum(cur_row * col + cur_col);
	floor[cur_row][cur_col] = 0;

	int cell_ori = (row - 2) * (col - 2);
	int cell = cell_ori;
	int tar_row, tar_col;

	while (cell > ((cell_ori * (100 - percent)) / 100) && clock_temp < 20000)
	{
		rp->get_rand(tar_row, tar_col);
		while (!(cur_row == tar_row && cur_col == tar_col))
		{
			if (tar_row > cur_row)
			{
				cur_row++;
			}
			else if (tar_row < cur_row)
			{
				cur_row--;
			}
			if (floor[cur_row][cur_col] != 0)
			{
				floor[cur_row][cur_col] = 0;
				cell--;
			}
			if (tar_col > cur_col)
			{
				cur_col++;
			}
			else if (tar_col < cur_col)
			{
				cur_col--;
			}
			if (floor[cur_row][cur_col] != 0)
			{
				floor[cur_row][cur_col] = 0;
				cell--;
			}
		}

		clock_temp -= clock();
		clock_temp *= -1;
	}

	int x[] = {-1, 1, 0, 0};
	int y[] = {0, 0, -1, 1};
	floor[R_row][R_col] = -2;
	for (int i = 0; i < 4; i++)
	{
		if (R_row + x[i] > 0 &&
				R_row + x[i] < row - 1 &&
				R_col + y[i] > 0 &&
				R_col + y[i] < col - 1 &&
				floor[R_row + x[i]][R_col + y[i]] == 0)
		{
			floor[R_row + x[i]][R_col + y[i]] = 1;
		}
	}

	int B = 0;
	bool flag = 1;
	int count = 1;
	while (flag == true)
	{
		flag = false;
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if (floor[i][j] == count)
				{
					for (int k = 0; k < 4; k++)
					{
						if (i + x[k] > 0 &&
								i + x[k] < row - 1 &&
								j + y[k] > 0 &&
								j + y[k] < col - 1 &&
								floor[i + x[k]][j + y[k]] == 0)
						{
							floor[i + x[k]][j + y[k]] = count + 1;
							if (B < count + 1)
							{
								B = count + 1;
							}
							flag = true;
						}
					}
				}
			}
		}
		count++;
	}
	B *= 2;
	B = (B * (rand() % 20 + 100)) / 100;

	ofstream file;
	file.open("floor.data");
	file << row << ' ' << col << ' ' << B << endl;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (i == R_row && j == R_col)
			{
				file << 'R';
			}
			else if (floor[i][j] == -1)
			{
				file << '1';
			}
			else
			{
				file << '0';
			}
		}
		if (i < row - 1)
		{
			file << endl;
		}
	}
	cout << "Real Rate = " << (cell_ori - cell) * 100 / cell_ori << "%" << endl;
}
