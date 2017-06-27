//
// Created by antchr-4 on 9/26/16.
//

#include <cstdio>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Nvx2Loader.h"


void Nvx2Loader::LoadNvx(char *file)
{
    int myfile = open(file, O_RDONLY);

    struct stat fileStat;
    if (fstat(myfile, &fileStat) < 0)
        return;

    if (myfile != -1)
    {

        uchar *ptr = (uchar *) mmap(0, fileStat.st_size, PROT_READ, MAP_SHARED, myfile, 0);

        if (ptr != MAP_FAILED)
        {
            printf("\n%s\n\n", "Loaded binary file into memory correctly!");
            Nvx2Header* nvxHeader = (Nvx2Header*) ptr;
            ptr += sizeof(Nvx2Header);
            if (nvxHeader->magic == 'NVX2')
            {
                this->numGroups = nvxHeader->numGroups;
                this->numVertices = nvxHeader->numVertices;
                this->vertexWidth = nvxHeader->vertexWidth;
                this->numIndices = nvxHeader->numIndices * 3;
                this->numEdges = nvxHeader->numEdges;
                this->vertexComponentMask = nvxHeader->vertexComponentMask;
                this->groupDataSize = 6 * sizeof(uint) * this->numGroups;
                this->vertexDataSize = this->numVertices * this->vertexWidth * sizeof(float);
                this->indexDataSize = this->numIndices * sizeof(int);

                this->groupDataPtr = nvxHeader + 1;
                this->vertexDataPtr = ((uchar*)this->groupDataPtr) + this->groupDataSize;
                this->indexDataPtr = ((uchar*)this->vertexDataPtr) + this->vertexDataSize;


                if (nvxHeader->numGroups > 0)
                {
                    for (int i = 0; i < nvxHeader->numGroups; ++i)
                    {
                        Nvx2Group* nvxGroup = (Nvx2Group*)ptr;
                        ptr += sizeof(Nvx2Group);

                        PrimitiveGroup pG;
                        pG.numVertices = nvxGroup->numVertices;
                        pG.baseIndex = nvxGroup->firstTriangle * 3;
                        pG.numIndices = nvxGroup->numTriangles * 3;
                        pG.primitiveTopology = Code::TriangleList;
                        this->primGroups.push_back(pG);
                    }
                }
                this->SetupVertexComponents();
            }
        }
    }

}

void Nvx2Loader::SetupVertexComponents()
{
    for (int i = 0; i < N2NumVertexComponents; i++)
    {
        VertexComponent::SemanticName sem;
        VertexComponent::Format fmt;

        int index = 0;
        if (vertexComponentMask & 1<<i)
        {
            switch (1<<i)
            {
                case N2Coord:        sem = VertexComponent::Position;     fmt = VertexComponent::Float3; break;
                case N2Normal:       sem = VertexComponent::Normal;       fmt = VertexComponent::Float3; break;
                case N2NormalB4N:    sem = VertexComponent::Normal;       fmt = VertexComponent::Byte4N; break;
                case N2Uv0:          sem = VertexComponent::TexCoord1;    fmt = VertexComponent::Float2; index = 0; break;
                case N2Uv0S2:        sem = VertexComponent::TexCoord1;    fmt = VertexComponent::Short2; index = 0; break;
                case N2Uv1:          sem = VertexComponent::TexCoord2;    fmt = VertexComponent::Float2; index = 1; break;
                case N2Uv1S2:        sem = VertexComponent::TexCoord2;    fmt = VertexComponent::Short2; index = 1; break;
                    /*
                case N2Uv2:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 2; break;
                case N2Uv2S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 2; break;
                case N2Uv3:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 3; break;
                case N2Uv3S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 3; break;
                */
                case N2Color:        sem = VertexComponent::Color;        fmt = VertexComponent::Float4; break;
                case N2ColorUB4N:    sem = VertexComponent::Color;        fmt = VertexComponent::UByte4N; break;
                case N2Tangent:      sem = VertexComponent::Tangent;      fmt = VertexComponent::Float3; break;
                case N2TangentB4N:   sem = VertexComponent::Tangent;      fmt = VertexComponent::Byte4N; break;
                case N2Binormal:     sem = VertexComponent::Binormal;     fmt = VertexComponent::Float3; break;
                case N2BinormalB4N:  sem = VertexComponent::Binormal;     fmt = VertexComponent::Byte4N; break;
                case N2Weights:      sem = VertexComponent::SkinWeights;  fmt = VertexComponent::Float4; break;
                case N2WeightsUB4N:  sem = VertexComponent::SkinWeights;  fmt = VertexComponent::UByte4N; break;
                case N2JIndices:     sem = VertexComponent::SkinJIndices; fmt = VertexComponent::Float4; break;
                case N2JIndicesUB4:  sem = VertexComponent::SkinJIndices; fmt = VertexComponent::UByte4; break;
                default:
                    printf("Invalid Nebula2 VertexComponent in Nvx2Loader");
                    sem = VertexComponent::Position;
                    fmt = VertexComponent::Float3;
                    break;
            }
            this->vertexComponents.push_back(VertexComponent(sem, index, fmt));
        }

    }

    this->SetupVertexBuffer();
    this->SetupIndexBuffer();

}

void Nvx2Loader::SetupVertexBuffer()
{

    for (int i = 0; i < this->numVertices; ++i)
    {
        //Vertex Float3
        vector3D vec;
        vec[0] = *(float*)vertexDataPtr;
        vertexDataPtr += sizeof(float);
        vec[1] = *(float*)vertexDataPtr;
        vertexDataPtr += sizeof(float);
        vec[2] = *(float*)vertexDataPtr;
        vertexDataPtr += sizeof(float);
        this->vbo.push_back(vec);

        //Normal Byte4N (value / 127.0f)
        vector3D vecN;
        vecN[0] = *(char*)vertexDataPtr / 127.0f;
        vertexDataPtr += sizeof(char);
        vecN[1] = *(char*)vertexDataPtr / 127.0f;
        vertexDataPtr += sizeof(char);
        vecN[2] = *(char*)vertexDataPtr / 127.0f;
        vertexDataPtr += sizeof(char);
        vertexDataPtr += sizeof(char);
        this->nbo.push_back(vecN);

        //UVs Float2
        vector3D vecUV;
        vecUV[0] = *(float*)vertexDataPtr;
        vertexDataPtr += sizeof(float);
        vecUV[1] = *(float*)vertexDataPtr;
        vertexDataPtr += sizeof(float);
        this->uvbo.push_back(vecUV);

        //Tangent Byte4N (value / 127.0f)
        vertexDataPtr += sizeof(char);
        vertexDataPtr += sizeof(char);
        vertexDataPtr += sizeof(char);
        vertexDataPtr += sizeof(char);

        //Binormal Byte4N (value / 127.0f)
        vertexDataPtr += sizeof(char);
        vertexDataPtr += sizeof(char);
        vertexDataPtr += sizeof(char);
        vertexDataPtr += sizeof(char);

        //SkinWeights UByte4N (value / 255.0f)
        vector4D  vecWeight;
        vecWeight[0] = *(uchar*)vertexDataPtr / 255.0f;
        vertexDataPtr += sizeof(uchar);
        vecWeight[1] = *(uchar*)vertexDataPtr / 255.0f;
        vertexDataPtr += sizeof(uchar);
        vecWeight[2] = *(uchar*)vertexDataPtr / 255.0f;
        vertexDataPtr += sizeof(uchar);
        vecWeight[3] = *(uchar*)vertexDataPtr / 255.0f;
        vertexDataPtr += sizeof(uchar);
        weightV.push_back(vecWeight);

        //Skin J Indices Ubyte4
        vector4D vecJ;
        vecJ[0] = *(uchar*)vertexDataPtr;
        vertexDataPtr += sizeof(uchar);
        vecJ[1] = *(uchar*)vertexDataPtr;
        vertexDataPtr += sizeof(uchar);
        vecJ[2] = *(uchar*)vertexDataPtr;
        vertexDataPtr += sizeof(uchar);
        vecJ[3] = *(uchar*)vertexDataPtr;
        vertexDataPtr += sizeof(uchar);
        this->jIndex.push_back(vecJ);
    }
}

void Nvx2Loader::SetupIndexBuffer()
{
    for (int i = 0; i < this->numIndices; ++i)
    {
        ibo.push_back((int*)indexDataPtr);
        indexDataPtr += sizeof(int);
    }
}
