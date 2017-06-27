//
// Created by antchr-4 on 9/26/16.
//

#ifndef GSCEPT_LAB_ENV_NVX2LOADER_H
#define GSCEPT_LAB_ENV_NVX2LOADER_H
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

#include <vector>
#include "PrimitiveGroup.h"
#include "VertexComponent.h"
#include "vector3D.h"
#include "vector4D.h"


//------------------------------------------------------------------------------
/**
    NVX2 file format structs.
    NOTE: keep all header-structs 4-byte aligned!
*/
struct Nvx2Header
{
    uint magic;
    uint numGroups;
    uint numVertices;
    uint vertexWidth;
    uint numIndices;
    uint numEdges;
    uint vertexComponentMask;
};

struct Nvx2Group
{
    uint firstVertex;
    uint numVertices;
    uint firstTriangle;
    uint numTriangles;
    uint firstEdge;
    uint numEdges;
};


class Nvx2Loader
{
public:
    void LoadNvx(char* file);
private:
    std::vector<PrimitiveGroup> primGroups;
    void SetupVertexComponents();
    void SetupVertexBuffer();
    void SetupIndexBuffer();

    /// Nebula2 vertex components, see Nebula2's nMesh2 class for details
    enum N2VertexComponent
    {
        N2Coord        = (1<<0),      // 3 floats
        N2Normal       = (1<<1),      // 3 floats
        N2NormalB4N   = (1<<2),      // 4 unsigned bytes, normalized
        N2Uv0          = (1<<3),      // 2 floats
        N2Uv0S2        = (1<<4),      // 2 shorts, 4.12 fixed point
        N2Uv1          = (1<<5),      // 2 floats
        N2Uv1S2        = (1<<6),      // 2 shorts, 4.12 fixed point
        N2Uv2          = (1<<7),      // 2 floats
        N2Uv2S2        = (1<<8),      // 2 shorts, 4.12 fixed point
        N2Uv3          = (1<<9),      // 2 floats
        N2Uv3S2        = (1<<10),     // 2 shorts, 4.12 fixed point
        N2Color        = (1<<11),     // 4 floats
        N2ColorUB4N    = (1<<12),     // 4 unsigned bytes, normalized
        N2Tangent      = (1<<13),     // 3 floats
        N2TangentB4N  = (1<<14),     // 4 unsigned bytes, normalized
        N2Binormal     = (1<<15),     // 3 floats
        N2BinormalB4N = (1<<16),     // 4 unsigned bytes, normalized
        N2Weights      = (1<<17),     // 4 floats
        N2WeightsUB4N  = (1<<18),     // 4 unsigned bytes, normalized
        N2JIndices     = (1<<19),     // 4 floats
        N2JIndicesUB4  = (1<<20),     // 4 unsigned bytes

        N2NumVertexComponents = 21,
        N2AllComponents = ((1<<N2NumVertexComponents) - 1),
    };

    //Base::ResourceBase::Usage usage;
    //Base::ResourceBase::Access access;

    bool rawMode;
    //std::vector<VertexBufferBase> vertexBuffer;
    //std::vector<IndexBufferBase> indexBuffer;
public:
    std::vector<vector3D> vbo;
    std::vector<vector3D> nbo;
    std::vector<vector3D> uvbo;
    std::vector<vector4D> weightV;
    std::vector<vector4D> jIndex;
private:





    std::vector<int*> ibo;



//    MemoryVertexBufferLoaderBase* vertexBufferLoader;
  //  MemoryIndexBufferLoaderBase* indexBufferLoader;

    //Util::Array<CoreGraphics::PrimitiveGroup> primGroups;
    void* mapPtr;
    void* groupDataPtr;
    void* vertexDataPtr;
    void* indexDataPtr;

    int groupDataSize;
    int vertexDataSize;
    int indexDataSize;

    uint numGroups;
    uint numVertices;
    uint vertexWidth;
    uint numIndices;
    uint numEdges;
    uint vertexComponentMask;
    std::vector<VertexComponent> vertexComponents;


};


#endif //GSCEPT_LAB_ENV_NVX2LOADER_H
