#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include "GameObject.h"

class AABBCollider;
class SphereCollider;

struct Cell {
	std::vector<GameObject*> GO;
	AABBCollider* boundary;
};

class BroadPhaseGrid
{
private:
	//was going to turn it into a cell array
	//std::vector<std::vector<std::vector<Cell>>> cells;
	
	std::vector<Cell> cells;
	int size, width, height, depth;

	int cellX, cellY, cellZ;

public:

	BroadPhaseGrid();
	BroadPhaseGrid(int screenWidth, int screenHeight, int screnDepth, int cellSize); // width and height refer to screen size
	~BroadPhaseGrid();

	Cell& GetCell(int x, int y, int z); //I want to be able to change the cell I am getting
};

