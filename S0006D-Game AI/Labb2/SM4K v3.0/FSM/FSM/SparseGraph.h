#pragma once
#include <vector>
#include <list>
#include <cassert>
#include <string>
#include <iostream>

template <class node_type, class edge_type>

class SparseGraph
{

	
	typedef edge_type EdgeType;
	typedef node_type NodeType;

	typedef std::vector<node_type> NodeVector;
	typedef std::list<edge_type> EdgeList;
	typedef std::vector<EdgeList> EdgeListVector;


private:

	NodeVector nodes;

	EdgeListVector edges;

	///The next valid node index
	int nextNodeIndex;

	///Checks if a edge already exists or not
	bool UniqueEdge(int from, int to);


public:


	//SparseGraph();
	SparseGraph() : nextNodeIndex(0){};

	std::vector<std::list<edge_type>> getEdgeListVec() const { return edges; }
	std::vector<std::list<edge_type>>& getEdgeListVecRef() { return edges; }

	NodeType& getNode(int i);
	EdgeType& getEdge(int from, int to);


	int getNextNodeIndex() const { return nextNodeIndex; }
	
	///Adds Node to graph
	int AddNode(NodeType n);
	///Remobe Node to graph
	void RemoveNode(int node);
	///Adds edge to graph
	void AddEdge(EdgeType e);
	///remove edge to graph
	void RemoveEdge(int from, int to);

	void  SetEdgeCost(int from, int to, double cost);

	int getNumNodes(){ return nodes.size(); };

	bool GraphEmpty(){ return nodes.empty() };

	
	NodeVector& getNodesRef() { return nodes; }
	NodeVector getNodes() const { return nodes; }
	void setNodes(NodeVector val) { nodes = val; }
};

template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::SetEdgeCost(int from, int to, double cost)
{
	assert((from < nodes.size()) && (to < nodes.size()) && "SetEdgeCost ERROR: invalid index");

	for (EdgeList::iterator it = m_Edges[from].begin(); it != edges[from].end(); ++it)
	{
		if (it->getIndexTo() == to)
		{
			it->setCost(cost);
			break;
		}
	}
}

template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::RemoveEdge(int from, int to)
{
	assert(from < (int)nodes.size() && (to < (int)nodes.size() && "RemoveEdge ERROR: invalid from/to values"));

	EdgeList::iterator it;

	for (it = edges[to].begin(); it != edges[to].end(); it++)
	{
		if (it->getIndexTo() == from)
		{
			it = edges[to].erase(it);
			break;
		}

	}
	for (it = edges[from].begin(); it != edges[from].end(); it++)
	{
		if (it->getIndexTo() == to)
		{
			it = edges[from].erase(it);
			break;
		}
	}

}

template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::RemoveNode(int n)
{
	assert(n < (int)nodes.size() && "removeNode ERROR: invalid node index");

	//set index to invalid (Not really removing it)
	nodes[n].setIndex(-1);

	for (EdgeList::iterator edgeIt = edges[n].begin(); edgeIt != edges[n].end(); edgeIt++)
	{
		for (EdgeList::iterator it = edges[edgeIt->getIndexTo()].begin(); it != edges[edgeIt->getIndexTo()].end(); ++it)
		{
			if (it->getIndexTo() == n)
			{
				it = edges[edgeIt->getIndexTo()].erase(it);
			}
		}
	}



}

template <class node_type, class edge_type>
int SparseGraph<node_type, edge_type>::AddNode(NodeType n)
{
	if (n.getIndex() < nodes.size())
	{
		assert(nodes[n.getIndex()].getIndex() == -1 && "AddNode ERROR: Adding node with bad ID");

		nodes[n.getIndex()] = n;

		return nextNodeIndex;
	}
	else
	{
		//assert(n.getIndex() == nextNodeIndex && "AddNode ERROR: invalid index");

		nodes.push_back(n);
		edges.push_back(EdgeList());

		return nextNodeIndex++;
	}
}

template <class node_type, class edge_type>
node_type& SparseGraph<node_type, edge_type>::getNode(int i)
{
	assert((i < (int)nodes.size()) && (i >= 0) && "getNode ERROR: invalid index");

	return nodes[i];
}

template <class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::AddEdge(edge_type e)
{
	assert((e.getIndexFrom() < nextNodeIndex) && (e.getIndexTo() < nextNodeIndex) && "AddEdge ERROR: invalid index");

	//If nodes exist
	if ((nodes[e.getIndexTo()].getIndex() != -1) && (nodes[e.getIndexFrom()].getIndex() != -1))
	{
		//Add edge if unique
		if (UniqueEdge(e.getIndexFrom(), e.getIndexTo()))
		{
			edges[e.getIndexFrom()].push_back(e);

			//undirected so add edge both ways
			edge_type newEdge = e;

			newEdge.setIndexTo(e.getIndexFrom());
			newEdge.setIndexFrom(e.getIndexTo());

			edges[e.getIndexTo()].push_back(newEdge);

			//delete newEdge;

		}
	}
}

template <class node_type, class edge_type>
edge_type& SparseGraph<node_type, edge_type>::getEdge(int from, int to)
{
	//assert((from < nodes.size()) && (from >= 0) && (node[from].getIndex() != -1) && "GetEdge Error!: ivalid 'from' index");

	//assert((to < nodes.size()) && (from >= 0) && (node[to].getIndex() != -1) && "GetEdge Error!: ivalid 'to' index");

	for (EdgeList::iterator it = edges[from].begin(); it != edges[from].end(); ++it)
	{
		if (it->getIndexTo() == to)
		{
			return *it;
		}
	}

	assert(0 && "getEdge ERROR: edge does not exist");
}

template <class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::UniqueEdge(int from, int to)
{
	for (EdgeList::iterator it = edges[from].begin(); it != edges[from].end(); ++it)
	{
		if (it->getIndexTo() == to)
		{
			return false;
		}
	}

	return true;
}



