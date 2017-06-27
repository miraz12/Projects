#include "AStar.h"
#include <queue>
#include "IndexedPriorityQLow.h"




AStar::~AStar()
{
}


void AStar::Search()
{
	IndexedPriorityQLow Q(graph->getNumNodes());

	Q.insert(startIndex);

	while (!Q.empty())
	{
		int NextClosestNode = Q.Pop();

		sPTree[NextClosestNode] = sFrontier[NextClosestNode];

		if (NextClosestNode == goalIndex)
		{
			return;
		}

		std::list<NavEdge>& edges = graph->getEdgeListVecRef()[NextClosestNode];
		for (std::list<NavEdge>::iterator it = edges.begin(); it != edges.end(); ++it)
		{
			double nHCost = AStar::Manhattandistance(it->getIndexTo(), goalIndex);
			double nGCost = (vecGCost[NextClosestNode] + it->getCost());
			te = it->getIndexTo();

			if (sFrontier[it->getIndexTo()] == NULL)
			{
				vecFCost[it->getIndexTo()] = nGCost + nHCost;
				vecGCost[it->getIndexTo()] = nGCost;

				Q.insert(it->getIndexTo());

				sFrontier[it->getIndexTo()] = &(*it);
			}

			else if ((nGCost < vecGCost[it->getIndexTo()]) && (sPTree[it->getIndexTo()] == NULL))
			{
				vecFCost[it->getIndexTo()] = nGCost + nHCost;
				vecGCost[it->getIndexTo()] = nGCost;

				Q.ChangePriority(it->getIndexTo());

				sFrontier[it->getIndexTo()] = &(*it);

			}
		}

	}

}

std::list<int> AStar::GetPathToTarget()
{
	std::list<int> nPath;
	
	if (goalIndex < 0)
	{
		return nPath;
	}

	int nd = goalIndex;

	nPath.push_front(nd);

	while ((nd != startIndex) && (sPTree[nd] != 0))
	{
		nd = sPTree[nd]->getIndexFrom();

		nPath.push_front(nd);
	}

	return nPath;



}


