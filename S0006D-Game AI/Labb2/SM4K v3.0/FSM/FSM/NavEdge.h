#pragma once
#include "GraphEdge.h"
#include <vector>
#include "flags.h"
///Node type, dosent contain anything really
class NavEdge :	public GraphEdge
{
private:
	///Node flag, not used in this lab
	int flag;

	

public:


	NavEdge(){};

	NavEdge(int from, int to, int cost, int flags = 0) : GraphEdge(from, to, cost), flag(flags){};

	virtual ~NavEdge(){};

	int getFlag() const { return flag; }
	void setFlag(int val) { flag = val; }


};

