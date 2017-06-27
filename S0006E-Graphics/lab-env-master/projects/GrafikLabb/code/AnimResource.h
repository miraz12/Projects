//
// Created by antchr-4 on 9/20/16.
//

#ifndef GSCEPT_LAB_ENV_ANIMRESOURCE_H
#define GSCEPT_LAB_ENV_ANIMRESOURCE_H

#include "AnimClip.h"
#include "AnimKeyBuffer.h"

class AnimResource
{
public:
    AnimResource();
    virtual ~AnimResource();

    void setupClips(int numClips);
    AnimKeyBuffer& SetupKeyBuffer(int numKeys);

public:
    AnimClip* animClips;
    AnimKeyBuffer* animKeyBuffer;
};


#endif //GSCEPT_LAB_ENV_ANIMRESOURCE_H
