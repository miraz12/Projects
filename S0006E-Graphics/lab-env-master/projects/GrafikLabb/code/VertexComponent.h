//
// Created by antchr-4 on 9/26/16.
//

#ifndef GSCEPT_LAB_ENV_VERTEXCOMPONENT_H
#define GSCEPT_LAB_ENV_VERTEXCOMPONENT_H


class VertexComponent
{
public:
    /// component semantic
    enum SemanticName
    {
        Position = 0,
        Normal = 1,
        TexCoord1 = 2,
        Tangent = 3,
        Binormal = 4,
        Color = 5,
        TexCoord2 = 6,
        SkinWeights = 7,
        SkinJIndices = 8,

        Invalid,
    };

    /// component format
    enum Format
    {
        Float,      //> one-component float, expanded to (float, 0, 0, 1)
        Float2,     //> two-component float, expanded to (float, float, 0, 1)
        Float3,     //> three-component float, expanded to (float, float, float, 1)
        Float4,     //> four-component float
        UByte4,     //> four-component unsigned byte
        Byte4,		//> four-component signed byte
        Short2,     //> two-component signed short, expanded to (value, value, 0, 1)
        Short4,     //> four-component signed short
        UByte4N,    //> four-component normalized unsigned byte (value / 255.0f)
        Byte4N,		//> four-component normalized signed byte (value / 127.0f)
        Short2N,    //> two-component normalized signed short (value / 32767.0f)
        Short4N,    //> four-component normalized signed short (value / 32767.0f)

        // PS3-specific
                Float16,
        Float16_2,
        Float16_3,
        Float16_4,
    };

    /// access type hint, this is only relevant on the Wii
    enum AccessType
    {
        None,
        Direct,     //> component has direct value (non-indexed)
        Index8,     //> component is indexed with 8-bit indices
        Index16,    //> component is indexed with 16-bit indices
        Index32,    //> component is indexed with 32-bit indices
    };

    /// stride type tells if the compoent should be per-instance or per-vertex
    enum StrideType
    {
        PerVertex,
        PerInstance
    };

    VertexComponent() :
            semName(Invalid),
            semIndex(0),
            format(Float),
            accessType(Index16),
            streamIndex(0),
            byteOffset(0),
            strideType(PerVertex),
            stride(0) { };

    VertexComponent(SemanticName semName, int semIndex, Format format, int streamIndex=0, StrideType strideType=PerVertex, int stride=0) :
            semName(semName),
            semIndex(semIndex),
            format(format),
            accessType(Index16),
            streamIndex(streamIndex),
            byteOffset(0),
            strideType(strideType),
            stride(stride) { };


    SemanticName semName;
    int semIndex;
    Format format;
    AccessType accessType;
    StrideType strideType;
    int stride;
    int streamIndex;
    int byteOffset;

};





#endif //GSCEPT_LAB_ENV_VERTEXCOMPONENT_H
