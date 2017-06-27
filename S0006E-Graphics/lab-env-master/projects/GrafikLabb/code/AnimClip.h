//
// Created by antchr-4 on 9/20/16.
//

#ifndef GSCEPT_LAB_ENV_ANIMCLIP_H
#define GSCEPT_LAB_ENV_ANIMCLIP_H

#include <string>
#include "AnimCurve.h"

/// anim infinity types
enum CodeClip
{
    Constant,       //> return first or last key
    Cycle,          //> loop the animation

    NumInfinityTypes,
    InvalidInfinityType,
};

class AnimClip
{
public:


    AnimClip();
    virtual ~AnimClip();


    void setupCurves(int numCurves);


public:
    std::string name;
    int startKeyIndex;
    int numKeys;
    int keyStride;
    float keyDuration;
    CodeClip preInfinityType;
    CodeClip postInfinityType;
    AnimCurve* curves;
    //-
    //Need events??
    //-
    int keySliceFirstKeyIndex;
    int keySliceByteSize;
    bool keySliceValuesValid;
    bool inBeginEvents;
    int size;
};


#endif //GSCEPT_LAB_ENV_ANIMCLIP_H
