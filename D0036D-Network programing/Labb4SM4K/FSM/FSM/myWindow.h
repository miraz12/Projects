#pragma once

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

//Network
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "structs.h"

//Shader loader
#include "loadShaders.h"

#include "Model.h"

#include "shapeGenerators.hpp"
#include "Object.h"
#include "Unit.h"
#include "GameHandler.h"
#include "networklistener.h"
#include "RC4.h"




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
	std::vector<Object*> objects;
public:
	//Standard functions**
	myWindow()
	{

		ConnectSocket = INVALID_SOCKET;
		client1 = NULL;
	};

	//Initialized GL
	int initializeGL();

	//Create window
	int createWindow();

	//Initalize scene
	void initializeScene();
	///Initialize network and setup encryption.
	void initializeNetwork();
	///Sends a move message depending on the string "dir".
	void sendMove(std::string dir);

	SOCKET ConnectSocket;
	Client* client1;
	Client* client2;

	///Getters & Setters
	RC4* getRc4In() { return rc4In; }
	RC4* getRc4Out() { return rc4Out; }

private:

	//install shaders
	void installShaders();

	//NON STANDARD FUNCTIONS
	void gameLoop();

	void handleEvents(Unit &player);

	int iResult;


	NetworkListener* nList;

	RC4* rc4In;
	RC4* rc4Out;
	const char* keyIn;
	const char* keyOut;



};

