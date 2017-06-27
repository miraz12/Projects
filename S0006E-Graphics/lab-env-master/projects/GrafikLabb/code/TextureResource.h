#pragma once
#include "GL/glew.h"
#include "SoftRender.h"



class TextureResource
{
public:
	/// constructor
	TextureResource();
	TextureResource(const char*);

	void loadFromRaz(SoftRender &s);
	/// destructor
	~TextureResource();


	void bind();

private:
	GLuint m_texture;

};