#pragma once
#include "GL/glew.h"
#include "vector3D.h"
#include "vector4D.h"
#include <vector>
#include <iostream>
#include <iosfwd>

struct HE_vert;
struct HE_face;

struct HE_edge
{
    HE_vert* vert;
    HE_edge* pair;
    HE_face* face;
    HE_edge* next;
};
struct HE_vert
{
    vector3D verts;
    vector3D uv;
    vector3D normal;
    HE_edge* edge;

};
struct HE_face
{
    HE_edge* edge;
};


class MeshResource 
	{
	public:
		/// constructor
		MeshResource();
		/// destructor
		~MeshResource();

		void Setup();
		void DrawCube();
		void DrawOBJ();
		bool loadOBJ(const char* filename);
		void DrawQuad();
        ///Convert list of vertices to halfedgemesh.
        void convertToHalfEdges(int times);
        void DrawHalfEdge();
        void catmullClark();


private:

        int subTimes;
        std::vector<int> midPointInd;
        std::vector<vector3D> vertices;
        std::vector<vector3D> faces;
		std::vector<vector3D> uvs;
		std::vector<vector3D> normals; // Won't be used at the moment.

        std::vector<vector4D> facesQuad;
        std::vector<vector4D> facesQuadUv;
        std::vector<vector4D> facesQuadNormal;

        int numVerts, numFaces, numEdges;
        int vertsIt, faceIt, edgesIt;
        HE_vert* HEVA;
        HE_edge* HEEA;
        HE_face* HEFA;

		float onethrid;
        float onefourth;

		int vertexAttributes, vertexBuffer, indexBuffer;

		GLuint vbo, vao, uvb, ebo ,nb;

	};
