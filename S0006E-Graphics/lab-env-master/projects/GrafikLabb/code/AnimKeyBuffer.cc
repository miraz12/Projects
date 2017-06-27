//
// Created by antchr-4 on 9/20/16.
//

#include "AnimKeyBuffer.h"

void AnimKeyBuffer::Setup(int numKeys)
{
    this->numKeys = numKeys;
    this->mapCount = 0;
    this->keyBuffer = (vector4D*)malloc(this->numKeys * sizeof(vector4D));
}

AnimKeyBuffer::~AnimKeyBuffer()
{
    free(keyBuffer);
}

void *AnimKeyBuffer::Map()
{
    this->mapCount++;
    return this->keyBuffer;
}

void AnimKeyBuffer::UnMap()
{
    this->mapCount--;
}
