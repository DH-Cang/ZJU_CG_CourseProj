#pragma once
#include <vector>
#include <iostream>

class maze_map
{
public:
	maze_map();
	~maze_map();

private:
	int row = 8;
	int column = 8;
	int MazeId[17][17];
	std::vector<int> Pos;
};
