#pragma once
class NavEdge
{
public:
	NavEdge();


	NavEdge(int index_from, int index_to, double cost) : indexFrom(index_from), indexTo(index_to), eCost(cost){}

	~NavEdge();


	///from node index
	int indexFrom;
	///To node index
	int indexTo;
	///Edge cost
	double eCost;

	int flag = 0;


};

