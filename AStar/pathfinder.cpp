#include "pathfinder.h"

int FindPath(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY,
    const unsigned char* pMap, const int nMapWidth, const int nMapHeight, int* pOutBuffer, const int nOutBufferSize)
{
    PathFinder* path = new PathFinder(nStartX, nStartY, nTargetX, nTargetY, pMap, nMapWidth, nMapHeight, pOutBuffer, nOutBufferSize);
    int dist = path->FindDistance();
    delete path;
    return dist;
}


PathFinder::PathFinder(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY,
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

    this->pOutBuffer = pOutBuffer;
    this->nOutBufferSize = nOutBufferSize;

}

int PathFinder::FindDistance()
{
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

int PathFinder::getX(int index) const
{
    return index % mapWidth;
}

int PathFinder::getY(int index) const
{
    return std::floor(index / mapWidth);
}

int PathFinder::getIndex(int x, int y) const
{
    return y * mapWidth + x;
}

int PathFinder::getDistance(int x, int y) const
{
    return std::max(x - goalX, goalX - x) + std::max(y - goalY, goalY - y);
}

void PathFinder::investigate(node n)
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

void PathFinder::findShortestPath(int dist, int* pOutBuffer, const int nOutBufferSize)
{
    int i = 1;
    int index = goalIndex;

    while (index != startIndex)
    {
        *(pOutBuffer + dist - i) = index;
        index = parents[index];
        ++i;
    }
}