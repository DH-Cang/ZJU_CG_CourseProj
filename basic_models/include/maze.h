#pragma once
#include <cstdio>
#include <iostream>
#include <vector>

class maze
{
public:
	maze();
	~maze();

private:
	int row = 8;
	int column = 8;
	int MazeId[17][17];
	std::vector<int> Pos;
};

