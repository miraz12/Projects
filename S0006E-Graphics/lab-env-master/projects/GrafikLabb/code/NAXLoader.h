//
// Created by antchr-4 on 9/16/16.
//

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

#ifndef GSCEPT_LAB_ENV_NAXLOADER_H
#define GSCEPT_LAB_ENV_NAXLOADER_H

#include "AnimCurve.h"
#include "AnimClip.h"
#include "AnimResource.h"

struct Nax3Header
{
    uint magic;
    uint numClips;
    uint numKeys;
};

struct Nax3Clip
{
    ushort numCurves;
    ushort startKeyIndex;
    ushort numKeys;
    ushort keyStride;
    ushort keyDuration;
    uchar preInfinityType;          // CoreAnimation::InfinityType::Code
    uchar postInfinityType;         // CoreAnimation::InfinityType::Code
    ushort numEvents;
    char name[50];                  // add up to 64 bytes size for Nax3Clip
};

struct Nax3AnimEvent
{
    char name[47];
    char category[15];
    ushort keyIndex;
};

struct Nax3Curve
{
    uint firstKeyIndex;
    uchar isActive;                 // 0 or 1
    uchar isStatic;                 // 0 or 1
    uchar curveType;                // CoreAnimation::CurveType::Code
    uchar _padding;                 // padding byte
    float staticKeyX;
    float staticKeyY;
    float staticKeyZ;
    float staticKeyW;
};


class NAXLoader
{
public:
    NAXLoader();
    void LoadNax(char* name);

    virtual ~NAXLoader();

public:
    AnimResource* animRe;

};


#endif //GSCEPT_LAB_ENV_NAXLOADER_H
