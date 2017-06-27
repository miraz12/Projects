#pragma once
#include "GL/glew.h"
#include <string>
#include "matrix4D.h"




class ShaderObject
	{
	public:
		/// constructor
		ShaderObject();
		ShaderObject(const std::string&, const std::string&);

		/// destructor
		~ShaderObject();

		std::string LoadFile(const std::string&);
		GLuint CreateShader(const std::string&, unsigned int);
		void bind();

		void modMatrix4fv(const char*, matrix4D);
		void modVector4f(const char*, vector4D);
		void modVector3f(const char* name, vector3D vec);

	private:
		static const unsigned int NUM_SHADERS = 2;
		static const unsigned int NUM_UNIFORMS = 3;

		GLuint program;
		GLuint vertexShader;
		GLuint pixelShader;

		GLuint shaders[NUM_SHADERS];
		GLuint uniforms[NUM_UNIFORMS];

	};
