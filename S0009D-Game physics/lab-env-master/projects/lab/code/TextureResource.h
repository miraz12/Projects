#pragma once
#include "GL/glew.h"



class TextureResource
{
public:
	/// constructor
	TextureResource();
	TextureResource(const char*);

	/// destructor
	~TextureResource();


	void bind();

private:
	GLuint m_texture;

};