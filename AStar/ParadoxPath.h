/*#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

int FindPath(const int nStartX, const int nStartY,
	const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
	int* pOutBuffer, const int nOutBufferSize);

struct node 
{
	int index;
	int fCost;
	int hCost;
	int parent;
};

bool operator <(const node& x, const node& y) 
{
	return x.fCost > y.fCost;
}

int getX(int index);
int getY(int index);
int getIndex(int x, int y);
int getDistance(int x, int y);
void investigate(node n);
void findShortestPath(int dist, int* pOutBuffer, const int nOutBufferSize);

int mapWidth, mapHeight;

const unsigned char* map;
std::vector<int> hCosts;
std::vector<int> parents;

std::priority_queue<node> prioQueue;

int goalIndex, startIndex;
int goalX, goalY;
bool targetFound = false;

int FindPath(const int nStartX, const int nStartY,	const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight, int* pOutBuffer, const int nOutBufferSize)
{
	mapWidth = nMapWidth;
	mapHeight = nMapHeight;
	map = pMap;

	hCosts.assign(nMapWidth*nMapHeight, -1);
	parents.assign(nMapWidth*nMapHeight, -1);

	std::priority_queue<node> p;
	prioQueue = p;

	goalIndex = getIndex(nTargetX, nTargetY);
	goalX = nTargetX;
	goalY = nTargetY;

	node startingNode;
	startIndex = getIndex(nStartX, nStartY);
	startingNode.index = startIndex;
	startingNode.fCost = 0;
	startingNode.hCost = 0;
	startingNode.parent = -1;

	prioQueue.push(startingNode);
	targetFound = false;

	while (!prioQueue.empty() && !targetFound) 
	{
		node n = prioQueue.top();
		prioQueue.pop();
		investigate(n);
	}

	if (hCosts[goalIndex] != -1 && hCosts[goalIndex] <= nOutBufferSize) 
	{
		findShortestPath(hCosts[goalIndex], pOutBuffer, nOutBufferSize);
		return hCosts[goalIndex];
	}
	else if (hCosts[goalIndex] > nOutBufferSize) 
	{
		return hCosts[goalIndex];
	}
	else 
	{
		return -1;
	}
}

int getX(int index) 
{
	return index % mapWidth;
}

int getY(int index) 
{
	return floor(index / mapWidth);
}

int getIndex(int x, int y) 
{
	return y*mapWidth + x;
}

int getDistance(int x, int y) 
{
	return std::max(x - goalX, goalX - x) + std::max(y - goalY, goalY - y);
}

void investigate(node n) 
{
	// check if the node is the target
	if (n.index == goalIndex) 
	{
		targetFound = true;
		hCosts[n.index] = n.hCost;
		parents[n.index] = n.parent;
		return;
	}

	// check wether the node was already visited
	if (hCosts[n.index] != -1) 
	{
		return;
	}

	// if this is the shortest path from the source to this node:
	int x = getX(n.index);
	int y = getY(n.index);

	// set parent and hCost
	hCosts[n.index] = n.hCost;
	parents[n.index] = n.parent;

	// visit node on the left
	if (0 <= x - 1) 
	{
		int newId = getIndex(x - 1, y);
		if (hCosts[newId] == -1 && map[newId] == 1) 
		{
			node newNode;
			newNode.index = newId;
			newNode.parent = n.index;
			newNode.hCost = n.hCost + 1;
			newNode.fCost = (n.hCost + 1) + getDistance(x - 1, y);
			prioQueue.push(newNode);
		}
	}
	// visit node on the right
	if (x + 1 < mapWidth) 
	{
		int newId = getIndex(x + 1, y);
		if (hCosts[newId] == -1 && map[newId] == 1) 
		{
			node newNode;
			newNode.index = newId;
			newNode.parent = n.index;
			newNode.hCost = n.hCost + 1;
			newNode.fCost = (n.hCost + 1) + getDistance(x + 1, y);
			prioQueue.push(newNode);
		}
	}
	// visit node below
	if (0 <= y - 1) 
	{
		int newId = getIndex(x, y - 1);
		if (hCosts[newId] == -1 && map[newId] == 1) 
		{
			node newNode;
			newNode.index = newId;
			newNode.parent = n.index;
			newNode.hCost = n.hCost + 1;
			newNode.fCost = (n.hCost + 1) + getDistance(x, y - 1);
			prioQueue.push(newNode);
		}
	}
	if (y + 1 < mapHeight) 
	{
		int newId = getIndex(x, y + 1);
		if (hCosts[newId] == -1 && map[newId] == 1) 
		{
			node newNode;
			newNode.index = newId;
			newNode.parent = n.index;
			newNode.hCost = n.hCost + 1;
			newNode.fCost = (n.hCost + 1) + getDistance(x, y + 1);
			prioQueue.push(newNode);
		}
	}
}

void findShortestPath(int dist, int* pOutBuffer, const int nOutBufferSize) 
{
	int i = 1;
	int index = goalIndex;

	while (index != startIndex) 
	{
		*(pOutBuffer + dist - i) = index;
		index = parents[index];
		++i;
	}
}*/