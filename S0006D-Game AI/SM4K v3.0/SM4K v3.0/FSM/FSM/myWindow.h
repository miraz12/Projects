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
	//Window height and width
	float windowHeight = GameHandler::getInstance()->getWindowHeight();
	float windowWidth = GameHandler::getInstance()->getWindowWidth();

	GLFWwindow* glfwWindow;

	//Ids and shit here
	GLuint programID;
	GLuint textShaderID;
	GLuint debugProgramID;

	//Time variables
	float deltaTime;

	std::vector<Object> map;
	std::vector<Object> objects;
public:
	//Standard functions**
	myWindow(){};

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

	//Pathfinding graph
	Pathfinder *test;
	int tileSize = 20;

};

