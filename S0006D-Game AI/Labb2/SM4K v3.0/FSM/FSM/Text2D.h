#pragma once
#include <vector>
#include <cstring>

#include <GL/glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "TextureLoader.h"

class Text2D
{
private:
	unsigned int windowWidth;
	unsigned int windowHeight;
	unsigned int Text2DTextureID;
	unsigned int Text2DVertexBufferID;
	unsigned int Text2DUVBufferID;
	unsigned int Text2DShaderID;
	unsigned int Text2DUniformID;
public:
	void initText2D(unsigned int text2DShader, const char * texturePath, unsigned int windowWidth, unsigned int windowHeight);
	void printText2D(const char * text, int x, int y, int size);
	void cleanupText2D();
};