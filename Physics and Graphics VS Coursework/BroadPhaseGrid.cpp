#include "BroadPhaseGrid.h"

BroadPhaseGrid::BroadPhaseGrid(int screenWidth, int screenHeight, int screenDepth, int cellSize)
{
	width = screenWidth;
	height = screenHeight;
	depth = screenDepth;
	size = cellSize;

	cellX = ceil((float)width / cellSize);
	cellY = ceil((float)height / cellSize);
	cellZ = ceil((float)depth / cellSize);

	//Allocates all the cells
	cells.resize((cellY * cellX) * cellZ);

	//was going to turn it into a cell array
	//cells.resize(cellY);
	//for (int )
}

BroadPhaseGrid::~BroadPhaseGrid()
{
}

Cell& BroadPhaseGrid::GetCell(int x, int y, int z)
{
	int i = (width * height * z) + (width * y);
	if (cells[i].boundary == nullptr)
	{
		cells[i].boundary = new AABBCollider(vec3(size, size, size), vec3(x,y,z), vec3(0,0,0), "Boundary");
	}
	return cells[i];
}
