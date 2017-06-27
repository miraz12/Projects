#pragma once
#include "SparseGraph.h"
#include "NavEdge.h"
#include "NavNode.h"
///A* search algorithm
class AStar
{
private:

	SparseGraph<NavNode, NavEdge>& graph;

	///Startnode and goal node index
	int startIndex, goalIndex;
	///Holds G and F cost
	std::vector<double> vecGCost;
	std::vector<double> vecFCost;

	///Shortest path tree
	std::vector< NavEdge*> sPTree;
	///SearchFrontier
	std::vector< NavEdge*> sFrontier;

	///Actual search function
	void Search();

public:
	AStar(SparseGraph<NavNode, NavEdge>& g, NavNode s, NavNode go) :graph(g), startIndex(s.getIndex()), goalIndex(go.getIndex()), vecFCost(graph.getNumNodes(), 0.0), vecGCost(graph.getNumNodes(), 0.0), sPTree(graph.getNumNodes()), sFrontier(graph.getNumNodes())
	{
		Search();
	};
	~AStar();

	std::vector<int> GetPathToTarget();
};

