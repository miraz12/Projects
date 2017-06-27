#pragma once

//Graph
#include "Pathfinder.h"
#include "SparseGraph.h"
#include "NavNode.h"
#include "NavEdge.h"
#include "BFSearch.h"

//Glew
#include <GL\glew.h>

//GLFW
#include <GLFW\glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

//Streams
#include <fstream>

//vector
#include <vector>

//Shader loader
#include "loadShaders.h"

#include "Model.h"

#include "shapeGenerators.hpp"
#include "Object.h"
#include "Unit.h"

class myWindow
{
//Private pointers and constants
private:
	//Projection matrix
	glm::mat4 Projection;

	//Window height and width
	float windowHeight;
	float windowWidth;

	GLFWwindow* glfwWindow;

	//Ids and shit here
	GLuint programID;
	GLuint textShaderID;
	GLuint debugProgramID;

	//Time variables
	float deltaTime;

	std::vector<Object> objects;
public:
	//Standard functions**
	myWindow(float windowWidth, float windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight) {};

	//Initialized GL
	int initializeGL();

	//Create window
	int createWindow();

	//Initalize scene
	void initializeScene();
private:

	//install shaders
	void installShaders();

	//NON STANDARD FUNCTIONS
	void gameLoop();

	void handleEvents(Unit &player);

	void drawGraph(int i);

	//Pathfinding graph
	Pathfinder test = Pathfinder();
	int tileSize = 20;


};

