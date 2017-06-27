#pragma once
#include <vector>
#include <list>
#include "NavEdge.h"
#include "NavNode.h"



class SparseGraph
{

private:
	std::vector<NavNode> m_Nodes;
	std::vector<std::list<NavEdge>> m_Edges;
	int m_iNextNodeIndex;

	int graphWidth, graphHeight;

	bool UniqueEdge(int from, int to);

public:
	SparseGraph();
	~SparseGraph();

	std::vector<std::list<NavEdge>> getEdgeListVec() const { return m_Edges; }
	std::vector<std::list<NavEdge>>& getEdgeListVecRef() { return m_Edges; }

	NavNode& getNode(int i);
	NavEdge& getEdge(int from, int to);

	///Generate SparseGraph
	void GenerateGraph(const unsigned char* pMap, int mMapWidth, int mMapHeight, int startX, int startY, int targetX, int targetY);
	bool ValidNeighbour(int x, int y, int gWidth, int gHeight);

	///Adds Node to graph
	int AddNode(NavNode n);
	///Adds edge to graph
	void AddEdge(NavEdge e);
	///Set edgecost
	void  SetEdgeCost(int from, int to, double cost);
	int getNumNodes(){ return m_Nodes.size(); };
	bool GraphEmpty(){ return m_Nodes.empty(); };
	int getNextNodeIndex() const { return m_iNextNodeIndex; }

	void AddNewEdges(int idx);

	double distance(int x1, int y1, int x2, int y2);
};



