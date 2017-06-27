//
// Created by antchr-4 on 2/1/17.
//

#ifndef GSCEPT_LAB_ENV_CELL_H
#define GSCEPT_LAB_ENV_CELL_H


#include "vector3D.h"

class Cell
{
public:

    enum cellType
    {
        AIR,
        FLUID,
        SOLID
    };
    Cell(cellType t, int lay);
    Cell(){};

    float pressure;
    vector3D velocity = vector3D();
    vector3D stagedVel = vector3D();
    vector3D position;
    Cell* rightN;
    Cell* leftN;
    Cell* upN;
    Cell* downN;
    Cell* inN;
    Cell* outN;

    bool allNeighbors;
    cellType type;
    int layer = -1;


};


#endif //GSCEPT_LAB_ENV_CELL_H
