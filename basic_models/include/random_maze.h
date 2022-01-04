#pragma once
#include <cstdio>
#include <iostream>
#include <vector>
#include <random>

class RandomMaze
{
public:
	RandomMaze();
	~RandomMaze();
	void constructRandomMaze();
	int getMazeInfo(int i, int j);

private:
	int row = 8;
	int column = 8;
	int MazeId[17][17];
};

