//
// Created by antchr-4 on 9/20/16.
//

#ifndef GSCEPT_LAB_ENV_ANIMKEYBUFFER_H
#define GSCEPT_LAB_ENV_ANIMKEYBUFFER_H


#include "vector4D.h"

class AnimKeyBuffer
{

public:
    int numKeys;
    unsigned int mapCount;
    vector4D* keyBuffer;

    void Setup(int numKeys);
    void* Map();
    void UnMap();

    virtual ~AnimKeyBuffer();
};


#endif //GSCEPT_LAB_ENV_ANIMKEYBUFFER_H
