#pragma once
#include "SparseGraph.h"
#include "NavEdge.h"
#include "NavNode.h"
///Depth first seach
class DFSearch
{
private:

	enum
	{
		visited
		, unvisited
		, no_parent_assigned

	};

	SparseGraph<NavNode, NavEdge>& graph;

	///Stores visited indecies
	std::vector<int> vecVisited;

	///holds route taken to the target.
	std::vector<int> path;
	///Spanning tree
	std::vector<NavEdge> spanningTree;

	NavNode startNode;
	int startIndex, goalIndex;

	///sets to true if goal found
	bool found;
	///Actual search
	bool Search();



public:
	DFSearch(SparseGraph<NavNode, NavEdge>& g, NavNode s) :graph(g), startNode(s), found(false), vecVisited(graph.getNumNodes(), unvisited), path(graph.getNumNodes(), no_parent_assigned)
	{
		startIndex = startNode.getIndex();
		found  = Search();
	};

	~DFSearch();

	bool getFound() const { return found; };

	std::vector<NavEdge> getSpanningTree() const { return spanningTree; }

	std::vector<int> GetPathToTarget();
};

