//
// Created by antchr-4 on 9/20/16.
//

#ifndef GSCEPT_LAB_ENV_ANIMCURVE_H
#define GSCEPT_LAB_ENV_ANIMCURVE_H

#include "vector4D.h"

/// animation curve types
enum CodeCurve
{
    Translation,    //> keys in curve describe a translation
    Scale,          //> keys in curve describe a scale
    Rotation,       //> keys in curve describe a rotation quaternion
    Color,          //> keys in curve describe a color
    Velocity,       //> keys describe a linear velocition
    Float4,         //> generic 4D key

    NumCurveTypes,
    InvalidCurveType,
};

class AnimCurve
{
public:
    AnimCurve();

    virtual ~AnimCurve();

public:
    vector4D staticKey;
    int firstKeyIndex;
    CodeCurve curveType;
    bool isActive;
    bool isStatic;

};


#endif //GSCEPT_LAB_ENV_ANIMCURVE_H
