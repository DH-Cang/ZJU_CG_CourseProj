#include "../include/random_maze.h"


RandomMaze::RandomMaze()
{
	int r = 2 * row + 1, c = 2 * column + 1;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
			MazeId[i][j] = 1;//首先全是墙
	}
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++) {
			MazeId[2 * i + 1][2 * j + 1] = 0; //中间空出空间
		}
	}

	//Prime algo: randomly open the up left wall
	//
	constructRandomMaze();
	printf("hello random_maze, you should be like\n");
	for (int i = 1; i < r - 1; i++)
	{
		for (int j = 1; j < c - 1; j++)
		{
			if (MazeId[i][j]) {
				printf("x");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}
void RandomMaze::constructRandomMaze() {
	int count = row * column;
	int accsize = 0;
	int acc[64], noacc[64]; //acc for accessed, and noacc for non-accessed
	//64 = count = row * column = 8 * 8

	int offR[4] = { -1, 1, 0, 0 };//偏移量，4个量分别表示上下左右
	int offC[4] = { 0, 0, 1, -1 };//
	int offS[4] = { -1, 1, row, -row };

	for (int i = 0; i < count; i++)
	{
		acc[i] = 0;
		noacc[i] = 0;//开始所有点没有被访问
	}

	//起点（一直在中心）
	acc[0] = 36;
	int pos = acc[0];

	//第一个点存入
	noacc[pos] = 1;
	while (accsize < count)
	{
		//取出当下的点
		int x = pos % row;
		int y = pos / row;
		int offpos = -1;//用来记录偏移量
		int dir = 0; //标记便宜的方向

		while (++dir < 5)
		{
			//随机访问最近的点
			int point = (rand() % (4 - 0)) + 0;//[0,4)
			int repos;
			int move_x, move_y;
			//计算位移方向
			repos = pos + offS[point];
			move_x = x + offR[point];
			move_y = y + offC[point];

			//判断位移是否合法
			if (move_y > -1 && move_x > -1 && move_x < row && move_y < column && repos >= 0 && repos < count && noacc[repos] != 1)
			{
				noacc[repos] = 1;
				acc[++accsize] = repos;
				pos = repos;
				offpos = point;
				//相邻的格子中间打通
				MazeId[2 * x + 1 + offR[point]][2 * y + 1 + offC[point]] = 0;
				break;
			}
			else
			{
				if (accsize == count -1)
				{
					return;
				}
				continue;
			}
		}

		if (offpos < 0)
		{//周边没有路了，从走过的路里重新找个起点
			int index = rand() % ((accsize + 1) - 0);
			pos = acc[index];
		}
	}
}
int RandomMaze:: getMazeInfo(int i, int j) {
	return MazeId[i][j];
}

RandomMaze::~RandomMaze()
{
}