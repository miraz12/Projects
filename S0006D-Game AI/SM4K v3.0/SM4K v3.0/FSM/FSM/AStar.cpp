#include "AStar.h"
#include <queue>
#include "IndexedPriorityQLow.h"




AStar::~AStar()
{
}


void AStar::Search()
{
	IndexedPriorityQLow Q(vecFCost, graph.getNumNodes());

	Q.insert(startIndex);

	while (!Q.empty())
	{
		int NextClosestNode = Q.Pop();

		sPTree[NextClosestNode] = sFrontier[NextClosestNode];

		if (NextClosestNode == goalIndex)
		{
			return;
		}

		std::list<NavEdge>& edges = graph.getEdgeListVecRef()[NextClosestNode];
		for (std::list<NavEdge>::iterator it = edges.begin(); it != edges.end(); ++it)
		{
			double nHCost = glm::distance(graph.getNode(goalIndex).getPos(), graph.getNode(it->getIndexTo()).getPos());
			double nGCost = vecGCost[NextClosestNode] + it->getCost();

			if (sFrontier[it->getIndexTo()] == NULL)
			{
				vecFCost[it->getIndexTo()] = nGCost + nHCost;
				vecGCost[it->getIndexTo()] = nGCost;

				Q.insert(it->getIndexTo());

				sFrontier[it->getIndexTo()] = &(*it);
			}
			else if ((nGCost < vecGCost[it->getIndexTo()]) && (sPTree[it->getIndexTo()] == NULL))
			{
				vecFCost[it->getIndexFrom()] = nGCost + nHCost;
				vecGCost[it->getIndexFrom()] = nGCost;

				Q.ChangePriority(it->getIndexTo());

				sFrontier[it->getIndexTo()] = &(*it);

			}
		}

	}

}

std::vector<int> AStar::GetPathToTarget()
{
	std::vector<int> nPath;
	
	if (goalIndex < 0)
	{
		return nPath;
	}

	int nd = goalIndex;

	nPath.push_back(nd);

	while ((nd != startIndex) && (sPTree[nd] != 0))
	{
		nd = sPTree[nd]->getIndexFrom();

		nPath.push_back(nd);
	}

	return nPath;



}


