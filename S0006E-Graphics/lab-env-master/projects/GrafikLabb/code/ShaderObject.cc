#include "ShaderObject.h"
#include <iostream>
#include <fstream>



ShaderObject::ShaderObject(const std::string& vertexFile, const std::string& fragmentFile)
{
	program = glCreateProgram();
	shaders[0] = CreateShader(LoadFile(vertexFile), GL_VERTEX_SHADER);
	shaders[1] = CreateShader(LoadFile(fragmentFile), GL_FRAGMENT_SHADER);

	for (int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(program, shaders[i]);
	}

	//Needed??
	/*glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");*/

	GLint shaderLogSize;
	glLinkProgram(program);
	glValidateProgram(program);

	//Get error log
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetProgramInfoLog(program, shaderLogSize, NULL, buf);
		printf("[PROGRAM LINK ERROR]: %s", buf);
		delete[] buf;
	}

}


ShaderObject::ShaderObject()
{

}



ShaderObject::~ShaderObject()
{
	//glDeleteProgram(program);

}



std::string ShaderObject::LoadFile(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load file: " << fileName << std::endl;
	}

	return output;
}


GLuint ShaderObject::CreateShader(const std::string& fileName, unsigned int type)
{
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

	const GLchar* p[1];
	p[0] = fileName.c_str();
	GLint lengths[1];
	lengths[0] = fileName.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	GLint shaderLogSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(shader, shaderLogSize, NULL, buf);
		printf("[SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	return shader;
}

void ShaderObject::bind()
{
	glUseProgram(program);
}

void ShaderObject::modMatrix4fv(const char* name, matrix4D mat)
{
	GLuint loc = glGetUniformLocation(this->program, name);
	glUniformMatrix4fv(loc, 1, GL_TRUE, (GLfloat*)mat.get());

}
void ShaderObject::modMatrix4fv(const char* name, matrix4D* mat, int i)
{
	GLuint loc = glGetUniformLocation(this->program, name);
	glUniformMatrix4fv(loc, i, GL_TRUE, (GLfloat*)mat->get());


}
void ShaderObject::modVector4f(const char* name, vector4D vec)
{
	GLuint loc = glGetUniformLocation(this->program, name);
	glUniform4f(loc, vec[0], vec[1], vec[2], vec[3]);
}

void ShaderObject::modVector3f(const char* name, vector3D vec)
{
	GLuint loc = glGetUniformLocation(this->program, name);
	glUniform3f(loc, vec[0], vec[1], vec[2]);
}

