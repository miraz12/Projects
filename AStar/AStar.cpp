//
// Created by antchr-4 on 11/4/16.
//

#include <complex>
#include "AStar.h"

AStar::AStar(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY, const unsigned char *pMap,
             const int nMapWidth, const int nMapHeight, int *pOutBuffer, const int nOutBufferSize)
{
    this->startX = nStartX;
    this->startY = nStartY;
    this->goalX = nTargetX;
    this->goalY = nTargetY;

    this->startIndex = nStartY*nMapWidth+nStartX;
    this->goalIndex = nTargetY*nMapWidth+nTargetX;

    this->cameFrom = new int[nMapHeight*nMapWidth];
    this->closedSet.reserve(nMapHeight*nMapWidth);
    this->openSet.reserve(nMapHeight*nMapWidth);
    openSet.push_back(this->startIndex);

    this->fscore = new float[nMapHeight*nMapWidth];
    this->gscore = new float[nMapHeight*nMapWidth];
    this->hscore = new float[nMapHeight*nMapWidth];


    this->gscore[this->startIndex] = 0;
    this->fscore[this->startIndex] = calculateHauristic(nStartX, nStartY);
    this->nextValidNodeIndex++;

    while(!this->openSet.empty())
    {

    }


}

float AStar::calculateHauristic(int x1, int y1)
{
    float hCost = (abs(y1 - goalY)) + (abs(x1 - goalX));
    return hCost;
}
