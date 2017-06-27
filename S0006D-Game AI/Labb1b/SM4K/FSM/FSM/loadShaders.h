#pragma once

//Max arguement
#include <windows.h>

//Glew
#include <GL\glew.h>

//Streams
#include <fstream>

//Standard library includes
#include <vector>
#include <string>

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);