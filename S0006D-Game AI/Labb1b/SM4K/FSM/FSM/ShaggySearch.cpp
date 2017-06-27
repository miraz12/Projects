#include "ShaggySearch.h"
#include <queue>




bool ShaggySearch::Search()
{
	std::queue<NavEdge> q;
	std::queue<NavEdge> q2;

	int startX = graph.getNode(startIndex).getPos().x;
	int startY = graph.getNode(startIndex).getPos().y;
	int goalX = graph.getNode(goalIndex).getPos().x;
	int goalY = graph.getNode(goalIndex).getPos().y;

	NavEdge d(startIndex, startIndex, 0);

	q.push(d);

	vecVisited[startIndex] = visited;

	while (!q.empty() || !q2.empty())
	{
		NavEdge next = d;
		if (!q.empty()){
			next = q.front();
			q.pop();
		}
		else{
			next = q2.front();
			q2.pop();
		}

		vecVisited[next.getIndexTo()] = next.getIndexFrom();

		if (next.getIndexTo() == goalIndex){
			return true;
		}


		std::list<NavEdge> edges = graph.getEdgeListVec()[next.getIndexTo()];

		for (std::list<NavEdge>::iterator it = edges.begin(); it != edges.end(); it++){
			if (vecVisited[it->getIndexTo()] == unvisited)
			{
				if (startX - goalX < 0){
					if (graph.getNode(next.getIndexFrom()).getPos().x - graph.getNode(it->getIndexFrom()).getPos().x < 0){
						q.push(*it);
					}
					else{
						q2.push(*it);
					}
				}
				if (startY - goalY < 0){
					if (graph.getNode(next.getIndexFrom()).getPos().y - graph.getNode(it->getIndexFrom()).getPos().y < 0){
						q.push(*it);
					}
					else{
						q2.push(*it);
					}
				}

				if (startX - goalX > 0){
					if (graph.getNode(next.getIndexFrom()).getPos().x - graph.getNode(it->getIndexFrom()).getPos().x > 0){
						q2.push(*it);
					}
					else{
						q.push(*it);
					}
				}
				if (startY - goalY > 0){
					if (graph.getNode(next.getIndexFrom()).getPos().y - graph.getNode(it->getIndexFrom()).getPos().y > 0){
						q2.push(*it);
					}
					else{
						q.push(*it);
					}
				}

				vecVisited[it->getIndexTo()] = visited;
			}
		}

	}

	return false;
}

std::vector<int> ShaggySearch::GetPathToTarget()
{
	std::vector<int> rPath;


	if (!found || goalIndex < 0)
		return rPath;

	int nd = goalIndex;
	rPath.push_back(nd);

	while (nd != startIndex)
	{
		nd = vecVisited[nd];
		rPath.push_back(nd);
	}
	return rPath;
}
