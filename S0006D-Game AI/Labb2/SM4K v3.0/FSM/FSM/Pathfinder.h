#pragma once
#include "flags.h"
#include "SparseGraph.h"
#include "NavNode.h"
#include "NavEdge.h"
#include <map>

///Loads map and creates a graph from it.
class Pathfinder
{

private:
	SparseGraph<NavNode, NavEdge> graph;
	SparseGraph<NavNode, NavEdge> discoveredGraph;

	
	int graphWidth, graphHeight, y, start, endpoint;

	NavNode startNode;
	NavNode goalNode;
	std::vector<flags> temp_tilelist;
	
	//Global coords
	std::vector<std::map<std::vector<int>, int>> coordsVec;

	///Used when setting a nodes edges to see if a neigbour is valid or not


public:
	Pathfinder();
	~Pathfinder();

	bool ValidNeighbour(int x, int y, int NumCellsX, int NumCellsY);

	static Pathfinder* Pathfinder::Instance()
	{
		static Pathfinder instance;

		return (&instance);
	}

	//Loads a map "filename" and creates a graph from it 
	void Load(char* filename);

	void setEdges();

	void addAllEdges(SparseGraph<NavNode, NavEdge>& graph, int row, int col, int numcellsx, int numcellsy);


	SparseGraph<NavNode, NavEdge>& getGraph() { return graph; }

	int getGraphWidth() const { return graphWidth; }

	int getGraphHeight() const { return graphHeight; }

	NavNode getStart() const { return startNode; }
	void setStart(NavNode val) { startNode = val; }

	NavNode getGoalNode() const { return goalNode; }
	void setGoalNode(NavNode val) { goalNode = val; }
	SparseGraph<NavNode, NavEdge>& getDiscoveredGraph() { return discoveredGraph; }
	void setDiscoveredGraph(SparseGraph<NavNode, NavEdge> val) { discoveredGraph = val; }

};

