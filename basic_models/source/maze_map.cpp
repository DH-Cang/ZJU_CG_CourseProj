#include "../include/maze_map.h"

maze_map::maze_map()
{


	maze_map::maze_map()
	{
		int r = 2 * row + 1, c = 2 * column + 1;
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				MazeId[i][j] = 1;//开始所有格子都是墙
			}
		}

		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < column; j++) {
				MazeId[2 * i + 1][2 * j + 1] = 0;//奇数格打出路来
			}
		}
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++) {
				if (MazeId[i][j]) {
					printf(" x");
				}
				else
				{
					printf(" ");
				}
			}
			printf("\n");
		}
	}

	maze_map::~maze_map()
	{
		row = 0;
		column = 0;
	}
}
