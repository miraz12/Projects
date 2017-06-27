#pragma once
#include "GL/glew.h"
#include "vector3D.h"
#include <vector>
#include <iostream>
#include <iosfwd>
#include "Nvx2Loader.h"

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
		void SetupNvx2(char *file);

	private:

		Nvx2Loader* lod;

		std::vector<vector3D> vertices;
		std::vector<vector3D> uvs;
		std::vector<vector3D> normals;

		std::vector<vector4D> weights;
		std::vector<vector4D> boneIndex;

		int vertexAttributes, vertexBuffer, indexBuffer;

		GLuint vbo, vao, uvb, ebo ,nb, wb, bib;

	};
