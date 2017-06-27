//
// Created by antchr-4 on 9/26/16.
//

#ifndef GSCEPT_LAB_ENV_PRIMITIVEGROUP_H
#define GSCEPT_LAB_ENV_PRIMITIVEGROUP_H


enum Code
{
    InvalidPrimitiveTopology,

    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip,

    // Xbox360-specific
    RectList,

    // Xbox360/PS3 specific
    QuadList,

    // PS3 specific
    QuadStrip,

    // DX11 specific
    PatchList
};

class PrimitiveGroup
{
public:
    PrimitiveGroup();

public:
    int baseVertex;
    int numVertices;
    int baseIndex;
    int numIndices;
    Code primitiveTopology;
    //Math::bbox boundingBox;
};


inline PrimitiveGroup::PrimitiveGroup() :
    baseVertex(0),
    numVertices(0),
    baseIndex(0),
    numIndices(0),
    primitiveTopology(Code::InvalidPrimitiveTopology)
{}

#endif //GSCEPT_LAB_ENV_PRIMITIVEGROUP_H
