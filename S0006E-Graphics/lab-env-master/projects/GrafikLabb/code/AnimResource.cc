//
// Created by antchr-4 on 9/20/16.
//

#include "AnimResource.h"

AnimResource::AnimResource()
{}

AnimResource::~AnimResource()
{

    delete[] animClips;
    delete animKeyBuffer;
}

void AnimResource::setupClips(int numClips)
{
    animClips = new AnimClip[numClips];
}

AnimKeyBuffer& AnimResource::SetupKeyBuffer(int numKeys)
{
    animKeyBuffer = new AnimKeyBuffer();
    animKeyBuffer->Setup(numKeys);

    return *this->animKeyBuffer;
}
