#include "../include/maze.h"

maze::maze()
{
	int  r = 2 * row + 1, c = 2 * column + 1;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++) {
			MazeId[i][j] = 1; //开始全部设为墙
		}
	}

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++) {
			MazeId[2 * i + 1][2 * j + 1] = 0; //设置初始空格
		}
	}

	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++) {
			if (MazeId[i][j])
			{
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

maze::~maze()
{
}