//
// Created by antchr-4 on 9/5/16.
//

#ifndef GSCEPT_LAB_ENV_JOINTSSTRUCTURE_H
#define GSCEPT_LAB_ENV_JOINTSSTRUCTURE_H

#include "XmlLoader.h"
#include "matrix4D.h"
#include "GraphicsNode.h"
#include "NAXLoader.h"
#include "Nvx2Loader.h"


class JointsStructure
{
public:
    virtual ~JointsStructure();
    JointsStructure();
    void drawSkeleton();
    void createInverseOriginal();
    void createSkeleton();
    void rotateSkeleton(int i, matrix4D mat);
    void sortBones();

    void PlayAnimation(int index, int clipIndex);

    AnimResource* an;



    matrix4D bones[21];
    matrix4D inverseOrg[21];

private:
    int i = 0;

    std::vector<joint> joints;
    std::vector<GraphicsNode> gNodes;
    XmlLoader a;
    NAXLoader nLoad;
    Nvx2Loader nvxLoad;
};


#endif //GSCEPT_LAB_ENV_JOINTSSTRUCTURE_H
