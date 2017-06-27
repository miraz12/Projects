#pragma once
#include "flags.h"
#include "SparseGraph.h"
#include "NavNode.h"
#include "NavEdge.h"
///Loads map and creates a graph from it.
class Pathfinder
{

private:
	SparseGraph<NavNode, NavEdge> graph;

	
	int graphWidth, graphHeight;

	NavNode startNode;
	NavNode goalNode;

	///Used when setting a nodes edges to see if a neigbour is valid or not
	bool ValidNeighbour(int x, int y, int NumCellsX, int NumCellsY);

public:
	Pathfinder();
	~Pathfinder();

	//Loads a map "filename" and creates a graph from it 
	void Load(char* filename);


	SparseGraph<NavNode, NavEdge> getGraph() const { return graph; }

	int getGraphWidth() const { return graphWidth; }

	int getGraphHeight() const { return graphHeight; }

	NavNode getStart() const { return startNode; }
	void setStart(NavNode val) { startNode = val; }

	NavNode getGoalNode() const { return goalNode; }
	void setGoalNode(NavNode val) { goalNode = val; }
};

