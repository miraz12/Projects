#pragma once
#include "SparseGraph.h"
#include "NavNode.h"
#include "NavEdge.h"
///Breadth first search
class BFSearch
{
private:

	enum
	{
		 visited
		,unvisited
		,no_parent_assigned

	};

	SparseGraph<NavNode, NavEdge>& graph;

	///Visited indecies
	std::vector<int> vecVisited;

	///holds route taken to the target.
	std::vector<int> path;
	///Spanning tree
	std::vector<NavEdge> spanningTree;

	NavNode startNode;
	int startIndex, goalIndex;
	

	bool found;
	///Actual search
	bool Search();



public:
	BFSearch(SparseGraph<NavNode, NavEdge>& g, NavNode s) : graph(g), found(false), vecVisited(graph.getNumNodes(), unvisited), path(graph.getNumNodes(), no_parent_assigned), startNode(s)
	{
		startIndex = s.getIndex();
		found = Search();
	};

	~BFSearch();
	bool getFound() const { return found; };

	std::vector<NavEdge> getSpanningTree() const { return spanningTree; }

	std::vector<int> GetPathToTarget();

};

