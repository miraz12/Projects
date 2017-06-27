//
// Created by antchr-4 on 11/4/16.
//
#pragma once


#include <vector>

class AStar
{
public:

    AStar(int *closedSet);

    AStar(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY, const unsigned char* pMap,
               const int nMapWidth, const int nMapHeight, int* pOutBuffer, const int nOutBufferSize);

private:

    int nextValidNodeIndex = 0;

    std::vector<int> closedSet;
    std::vector<int> openSet;
    int* cameFrom;

    float* gscore;
    float* hscore;
    float* fscore;

    float calculateHauristic(int x, int y);

    int startIndex;
    int goalIndex;
    int startX;
    int startY;
    int goalX;
    int goalY;
};

