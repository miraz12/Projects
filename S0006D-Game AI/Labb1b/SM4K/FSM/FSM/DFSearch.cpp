#include "DFSearch.h"
#include <stack>

DFSearch::~DFSearch()
{
}

bool DFSearch::Search()
{
	std::stack<NavEdge> stack;

	NavEdge temp(startIndex, startIndex, 0);

	stack.push(temp);

	while (!stack.empty())
	{
		NavEdge next = stack.top();

		stack.pop();

		path[next.getIndexTo()] = next.getIndexFrom();

		if (next.getIndexTo() != temp.getIndexTo() || next.getIndexFrom() != temp.getIndexFrom())
		{
			spanningTree.push_back(next);
		}

		vecVisited[next.getIndexTo()] = visited;

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
				stack.push(*it);
			}
		}
	}
	return false;


}

std::vector<int> DFSearch::GetPathToTarget()
{
	std::vector<int> rPath;

	//just return an empty path if no path to target found or if
	//no target has been specified
	if (!found || goalIndex < 0) return rPath;

	int nd = goalIndex;

	rPath.push_back(nd);

	while (nd != startIndex)
	{
		nd = path[nd];

		rPath.push_back(nd);
	}

	return rPath;
}
