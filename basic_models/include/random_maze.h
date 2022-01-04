#pragma once
#include <cstdio>
#include <iostream>
#include <vector>

class RandomMaze
{
public:
	RandomMaze();
	~RandomMaze();

private:
	int row = 8;
	int column = 8;
	int MazeId[17][17];
};

