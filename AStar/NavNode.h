#pragma once
#include <vector>

class NavNode
{
public:
	NavNode();
	~NavNode();

	int index = -1;

	///Node flag
	int flag;
	int x;
	int y;

	void setPos(int ix, int iy) { x = ix; y = iy; }
};

