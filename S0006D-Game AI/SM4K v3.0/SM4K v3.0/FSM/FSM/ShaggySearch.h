#pragma once
#include "SparseGraph.h"
#include "NavNode.h"
#include "NavEdge.h"
//My own seach algorithm, based on BFS
class ShaggySearch
{
private:
	//Enums for 
	enum
	{
		 visited
		, unvisited
		, no_parent_assigned

	};

	SparseGraph<NavNode, NavEdge>& graph;

	///Stores visited indecies
	std::vector<int> vecVisited;
	
	std::vector<int> path;

	//Spanning tree containing all visited nodes
	std::vector<NavEdge> spanningTree;

	NavNode startNode;
	int startIndex, goalIndex;

	///Sets to true if goal is found
	bool found;
	///actual seach alg
	bool Search();



public:
	ShaggySearch(SparseGraph<NavNode, NavEdge>& g, NavNode s, NavNode go) : graph(g), found(false), vecVisited(graph.getNumNodes(), unvisited), startIndex(s.getIndex()), goalIndex(go.getIndex())
	{
		startIndex = s.getIndex();
		found = Search();
	};

	~ShaggySearch(){};

	bool getFound() const { return found; };

	std::vector<NavEdge> getSpanningTree() const { return spanningTree; }

	//Get path found to target
	std::vector<int> GetPathToTarget();

};

