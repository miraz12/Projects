/*#include "AStar.h"

int FindPath(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY, const unsigned char* pMap,
	const int nMapWidth, const int nMapHeight, int* pOutBuffer, const int nOutBufferSize)
{

	SparseGraph graph;

	graph.GenerateGraph(pMap, nMapWidth, nMapHeight, nStartX, nStartY, nTargetX, nTargetY);

	AStar* as = new AStar(graph, graph.getNode((nStartY * nMapWidth) + nStartX), graph.getNode((nTargetY * nMapWidth) + nTargetX), pOutBuffer, nOutBufferSize, pMap, nMapWidth, nMapHeight);

	return 0;
}
*/