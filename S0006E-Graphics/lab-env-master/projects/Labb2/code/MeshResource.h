#pragma once
#include "GL/glew.h"
#include "vector3D.h"
#include <vector>
#include <iostream>
#include <iosfwd>


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




	private:

		std::vector<vector3D> vertices;
		std::vector<vector3D> uvs;
		std::vector<vector3D> normals; // Won't be used at the moment.

		int vertexAttributes, vertexBuffer, indexBuffer;

		GLuint vbo, vao, uvb, ebo ,nb;

	};
