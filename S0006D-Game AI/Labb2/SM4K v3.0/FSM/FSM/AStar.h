#pragma once
#include "SparseGraph.h"
#include "NavEdge.h"
#include "NavNode.h"


///A* search algorithm
class AStar
{
private:

	int te = 0;

	SparseGraph<NavNode, NavEdge>* graph;

	///Startnode and goal node index
	int startIndex, goalIndex;
	///Holds G and F cost
	std::vector<double> vecGCost;
	std::vector<double> vecFCost;

	///Shortest path tree
	std::vector< const NavEdge*> sPTree;
	///SearchFrontier
	std::vector< const NavEdge*> sFrontier;

	///Actual search function
	void Search();

public:
	AStar(SparseGraph<NavNode, NavEdge>& inGraph, NavNode inStart, NavNode inGoal) :graph(&inGraph), startIndex(inStart.getIndex()), goalIndex(inGoal.getIndex()), vecFCost(graph->getNumNodes(), 0.0f), vecGCost(graph->getNumNodes(), 0.0f), sPTree(graph->getNumNodes()), sFrontier(graph->getNumNodes())
	{
		Search();
	};
	~AStar();

	float Manhattandistance(int source, int target){
		float sourceX = graph->getNode(source).getPos().x;
		float targetX = graph->getNode(target).getPos().x;

		float dx = abs(sourceX - targetX);

		float sourceY = graph->getNode(source).getPos().y;
		float targetY = graph->getNode(target).getPos().y;

		float dy = abs(sourceY - targetY);

		return (dx + dy);

		
	}

	std::list<int> GetPathToTarget();
};

