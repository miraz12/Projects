#pragma once
#include <queue>
#include <cmath>

struct node
{
    int index;
    int fCost;
    int hCost;
    int parent;


};

inline bool operator <(const node& x, const node& y)
{
    return x.fCost > y.fCost;
}

int FindPath(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY,
    const unsigned char* pMap, const int nMapWidth, const int nMapHeight, int* pOutBuffer, const int nOutBufferSize);

class PathFinder
{

public:

    PathFinder(const int nStartX, const int nStartY,
        const int nTargetX, const int nTargetY,
        const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
        int* pOutBuffer, const int nOutBufferSize);

    int FindDistance();

private:

    int getX(int index) const;
    int getY(int index) const;
    int getIndex(int x, int y) const;
    int getDistance(int x, int y) const;
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
    int* pOutBuffer;
    int nOutBufferSize;

};
