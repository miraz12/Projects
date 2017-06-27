#include "SparseGraph.h"
#include <cassert>
#include "flags.h"
#include <math.h>

SparseGraph::SparseGraph() : m_iNextNodeIndex(0)
{
}


SparseGraph::~SparseGraph()
{
}

bool SparseGraph::UniqueEdge(int from, int to)
{
	for (std::list<NavEdge>::iterator it = m_Edges[from].begin();
		it != m_Edges[from].end();
		++it)
	{
		if (it->indexTo == to)
		{
			return false;
		}
	}
	return true;
}

NavNode& SparseGraph::getNode(int idx)
{
	assert((idx < (int)m_Nodes.size()) && (idx >= 0) && "getNode ERROR: invalid index");

	return m_Nodes[idx];
}

NavEdge& SparseGraph::getEdge(int from, int to)
{
	assert((from < m_Nodes.size()) && (from >= 0) && m_Nodes[from].index != -1 && "GetEdge Error!: ivalid 'from' index");

	assert((to < m_Nodes.size()) && (to >= 0) && m_Nodes[to].index != -1 && "GetEdge Error!: ivalid 'to' index");

	for (std::list<NavEdge>::iterator it = m_Edges[from].begin(); it != m_Edges[from].end(); ++it)
	{
		if (it->indexTo == to) 
			return *it;
	}

	assert(0 && "getEdge ERROR: edge does not exist");
}
void SparseGraph::AddEdge(NavEdge edge)
{
	//first make sure the from and to nodes exist within the graph 
	assert((edge.indexFrom < m_iNextNodeIndex) && (edge.indexTo < m_iNextNodeIndex) && & "AddEdge ERROR: invalid index");

	//make sure both nodes are active before adding the edge
	if ((m_Nodes[edge.indexTo].index != -1) &&
		(m_Nodes[edge.indexFrom].index != -1))
	{
		//add the edge, first making sure it is unique
		if (UniqueEdge(edge.indexFrom, edge.indexTo))
		{
			m_Edges[edge.indexFrom].push_back(edge);
		}

		//if the graph is undirected we must add another connection in the opposite
		//direction
		
		//check to make sure the edge is unique before adding
		if (UniqueEdge(edge.indexTo, edge.indexFrom))
		{
			NavEdge NewEdge = edge;

			NewEdge.indexTo = edge.indexFrom;
			NewEdge.indexFrom = edge.indexTo;

			m_Edges[edge.indexTo].push_back(NewEdge);
		}
	}
}



void SparseGraph::SetEdgeCost(int from, int to, double newCost)
{
	//make sure the nodes given are valid
	assert((from < (int)m_Nodes.size()) && (to < (int)m_Nodes.size()) &&
		"<SparseGraph::SetEdgeCost>: invalid index");

	//visit each neighbour and erase any edges leading to this node
	for (std::list<NavEdge>::iterator it = m_Edges[from].begin(); it != m_Edges[from].end(); ++it)
	{
		if (it->indexTo == to)
		{
			it->eCost = (newCost);
			break;
		}
	}
}

int SparseGraph::AddNode(NavNode node)
{
	if (node.index < (int)m_Nodes.size())
	{
		//make sure the client is not trying to add a node with the same ID as
		//a currently active node
		assert(m_Nodes[node.index].index == -1 && "<SparseGraph::AddNode>: Attempting to add a node with a duplicate ID");

		m_Nodes[node.index] = node;

		return m_iNextNodeIndex;
	}

	else
	{
		//make sure the new node has been indexed correctly
		assert(node.index == m_iNextNodeIndex && "<SparseGraph::AddNode>:invalid index");

		m_Nodes.push_back(node);
		m_Edges.push_back(std::list<NavEdge>());

		return m_iNextNodeIndex++;
	}
}

void SparseGraph::GenerateGraph(const unsigned char* pMap, int mMapWidth, int mMapHeight,int startX, int startY, int targetX, int targetY)
{
	this->m_Nodes.reserve(mMapWidth*mMapHeight);

	int x = 0;
	int y = 0;

	int it = 0;
	for (y = 0; y < mMapHeight; y++)
	{

		for (x = 0; x < mMapWidth; x++)
		{
			NavNode node;
			node.index = (this->getNextNodeIndex());
			node.setPos(x, y);

			//Walkable
			if (pMap[it] == 1)
			{
				node.flag = walkable;
			}
			//Unwalkable
			if (pMap[it] == 0)
			{
				node.flag = unwalkable;
			}
			if (x == startX && y == startY)
			{
				node.flag = start;
			}
			if (x == targetX && y == targetY)
			{
				node.flag = goal;
			}
			this->AddNode(node);

			it++;
		}
	}

	graphWidth = x;
	graphHeight = y;

	
	/*for (int i = 0; i < this->getNumNodes() - 1; i++)
	{
		AddNewEdges(i);
	}*/
}

bool SparseGraph::ValidNeighbour(int x, int y, int gWidth, int gHeight)
{
	return !((x < 0) || (x >= gWidth) || (y < 0) || (y >= gHeight));
}

void SparseGraph::AddNewEdges(int idx)
{
	NavNode node = this->getNode(idx);

	if (this->getNode(idx).flag == unwalkable)
		return;

	int col = node.x;
	int row = node.y;

	for (int j = -1; j < 2; ++j)
	{
		for (int k = -1; k < 2; ++k)
		{
			int neighborX = col + k;
			int neighborY = row + j;

			//Check if neighbor is diagonal, if so continue.
			//if ((j == 0) && (k == 0)) continue; //Want to add diagonal nodes?? Use this.
			if ((k == 0) && (j == -1) || (k == -1) && (j == 0) || (k == 1) && (j == 0) || (k == 0) && (j == 1))
			{
				if ((ValidNeighbour(neighborX, neighborY, graphWidth, graphHeight)))
				{
					//Check neighboring node so it is walkable
					if (this->getNode(neighborY*graphWidth + neighborX).flag == unwalkable)
						continue;

					//distance not needed when not moving diagonally. Set eCost for all edges to 0
					double dist = 0.0;
					NavEdge newEdge(idx, neighborY*graphWidth + neighborX, dist);
					this->AddEdge(newEdge);

					//Non directed graph so add edges both ways
					NavEdge NewEdgeBack(neighborY*graphWidth + neighborX, row*graphWidth + col, dist);
					this->AddEdge(NewEdgeBack);
				}
			}
		}
	}
}

double SparseGraph::distance(int x1, int y1, int x2, int y2)
{
	double ySeparation = y2 - y1;
	double xSeparation = x2 - x1;

	return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}
