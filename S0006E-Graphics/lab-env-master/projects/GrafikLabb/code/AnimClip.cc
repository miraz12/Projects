//
// Created by antchr-4 on 9/20/16.
//

#include "AnimClip.h"

AnimClip::AnimClip()
{}

AnimClip::~AnimClip()
{
    delete[] curves;
}

void AnimClip::setupCurves(int numCurves)
{
    curves = new AnimCurve[numCurves];
}
