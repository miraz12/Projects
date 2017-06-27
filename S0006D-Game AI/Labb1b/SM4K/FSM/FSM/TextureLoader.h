#pragma once
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class TextureLoader
{
public:
	// Load a .BMP file using our custom loader
	static GLuint loadBMP(const char * imagepath);

	// Load a .DDS file using GLFW's own loader
	static GLuint loadDDS(const char * imagepath);
};