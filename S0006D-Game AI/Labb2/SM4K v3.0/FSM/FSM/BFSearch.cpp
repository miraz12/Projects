#include "BFSearch.h"
#include <queue>



bool BFSearch::Search()
{
	std::queue<NavEdge> Q;

	NavEdge temp(startIndex, startIndex, 0);

	Q.push(temp);

	vecVisited[startIndex] = visited;

	while (!Q.empty())
	{
		NavEdge next = Q.front();

		Q.pop();

		path[next.getIndexTo()] = next.getIndexFrom();

		if (next.getIndexTo() != temp.getIndexTo() || next.getIndexFrom() != temp.getIndexFrom())
		{
			spanningTree.push_back(next);
		}

		if (graph.getNode(next.getIndexTo()).getFlag() == goal)
		{
			goalIndex = next.getIndexTo();
			return true;
			
		}


		std::list<NavEdge> eVec = graph.getEdgeListVec()[next.getIndexTo()];	
		for (std::list<NavEdge>::iterator it = eVec.begin(); it != eVec.end(); ++it)
		{
			if (vecVisited[it->getIndexTo()] == unvisited)
			{
				Q.push(*it);

				vecVisited[it->getIndexTo()] = visited;
			}
			

		}

	}

	return false;
}

BFSearch::~BFSearch()
{
	
}

std::vector<int> BFSearch::GetPathToTarget()
{
	std::vector<int> rPath;

	//just return an empty path if no path to target found or if
	//no target has been specified
	if (!found || goalIndex < 0) return rPath;

	int nd = goalIndex;

	rPath.push_back(nd);

	while (nd != startIndex)
	{
		nd = this->path[nd];

		rPath.push_back(nd);
	}

	return rPath;

}
