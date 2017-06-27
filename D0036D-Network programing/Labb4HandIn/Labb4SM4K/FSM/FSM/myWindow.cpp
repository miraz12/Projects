#pragma once
#include "myWindow.h"
#include "Text2D.h"
#include "RTSCamera.h"
#include <thread>


#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define DEFAULT_PORT "49152"
#define DEFAULT_BUFLEN 1024


//Standard functions ****************************************

int myWindow::initializeGL() {
	//Initialise GLFW 
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialise GLFW");
		return -1;
	}
	return 0;
}

int myWindow::createWindow() {
	glfwWindowHint(GLFW_SAMPLES, 4); //Antialiasing 4x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //OpenGL 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Try to create a window, if failed, abort and return -1
	glfwWindow = glfwCreateWindow((int)windowWidth, (int)windowHeight, "AI example", NULL, NULL);
	if( glfwWindow == NULL ) {
		fprintf(stderr, "Failed to create window" );
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(glfwWindow);
	glewExperimental = true;
	if ( glewInit() != GLEW_OK ) {
		fprintf(stderr, "GLEW failed to initialise");
		return -1;
	}

	//Capture keys
	glfwSetInputMode(glfwWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetScrollCallback(glfwWindow, RTSCamera::ScrollZoom);
	//glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		//SHOW/HIDE CURSOR!

	return 0;
}



void myWindow::initializeScene() {
	//Do everthing in order here instead of calling in main

	GameHandler* gameHandler = GameHandler::getInstance();
	gameHandler->setMap("map.txt");

	//initializeNetwork();
	installShaders();
	//Use installed shader
	glUseProgram(programID);
	//Start running game loop


	gameLoop();


}
void myWindow::initializeNetwork()
{
	//Set in and out keys for RC4
	keyIn = "Server";
	keyOut = "Client";

	//Initialize RC4.
	rc4In = new RC4(keyIn, 6);
	rc4Out = new RC4(keyOut, 6);

	//Create clients which holds info about this and enemy player.
	client1 = new Client;
	client2 = new Client;

	//structure contains information about the Windows Sockets implementation.
	WSADATA wsaData;
	iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//-----------------------------------------Change to the desired IP---------------------------------
	std::string sargv = "130.240.40.26";
	//std::string sargv = "127.0.0.1";
	const char* argv = sargv.c_str();

	iResult = getaddrinfo(argv, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}

	ConnectSocket = INVALID_SOCKET;

	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}

}


void myWindow::sendMove(std::string dir)
{
	MoveEvent move;

	char sendbuf[sizeof(move)];

	move.event.type = Move;
	//filling move message
	move.event.head.length = sizeof(move);
	move.event.head.seq_no = client1->seq_nr++;
	move.event.head.id = client1->client_id;
	move.event.head.type = Event;

	if (dir == "left")
	{
		Coordinate newPos;
		newPos.x = client1->pos.x - 1;
		newPos.y = client1->pos.y;
		move.pos = newPos;
		move.dir = newPos;
	}
	else if (dir == "right")
	{
		Coordinate newPos;
		newPos.x = client1->pos.x + 1;
		newPos.y = client1->pos.y;
		move.pos = newPos ;
		move.dir = newPos;
	}
	else if (dir == "up")
	{
		Coordinate newPos;
		newPos.x = client1->pos.x;
		newPos.y = client1->pos.y + 1;
		move.pos = newPos;
		move.dir = newPos;
	}
	else if (dir == "down")
	{
		Coordinate newPos;
		newPos.x = client1->pos.x;
		newPos.y = client1->pos.y - 1;
		move.pos = newPos;
		move.dir = newPos;
	}

	//copying move message to sendbuf
	memcpy((void*)sendbuf, (void*)&move, sizeof(move));

	//Encrypt
	rc4Out->Encrypt(sendbuf, sizeof(sendbuf));

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, sizeof(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}

	printf("Bytes Sent: %ld\n", iResult);
}

void myWindow::installShaders() {
	programID = LoadShaders("vertexShader.gsl", "fragmentShader.gsl");
	textShaderID = LoadShaders("TextVertexShader.gsl", "TextFragmentShader.gsl");
}

void myWindow::gameLoop() 
{
	GameHandler* gameHandler = GameHandler::getInstance();

	Model squari = shapeGenerator::square((float)gameHandler->tileSize, (float)gameHandler->tileSize, programID);

	unsigned int mapwidth = gameHandler->getMap().width;
	unsigned int mapheight = gameHandler->getMap().height;

	Map tempMap = gameHandler->getMap();

	for (unsigned int x = 0; x < mapwidth; x++)
	{
		for (unsigned int y = 0; y < mapheight; y++)
		{			
			Unit squariObject = Unit(glm::vec3(0.0f, 0.0f, 0.0f)); // TEMP

			if (tempMap.getTile(x, y).passable == false)
				squariObject = (glm::vec3(1.0f, 0.0f, 0.0f));
			else if (tempMap.getTile(x, y).start == true)
				squariObject = (glm::vec3(0.0f, 1.0f, 0.0f));
			else if (tempMap.getTile(x, y).goal == true)
				squariObject = (glm::vec3(0.0f, 1.0f, 1.0f));
			else
				squariObject = (glm::vec3(0.5f, 0.5f, 0.5f));

			squariObject.setModel(&squari);
			squariObject.setPosition((GLfloat)((x*gameHandler->tileSize)), (GLfloat)((y*gameHandler->tileSize)));
			map.push_back(squariObject);
		}
	}

	Model triangle = shapeGenerator::square(40, 25, programID);
	Unit triangleObject = Unit(glm::vec3(0.4f, 0.1f, 0.2f));
	triangleObject.setModel(&triangle);
	triangleObject.setPosition(500, 600);
	objects.push_back(&triangleObject);
	client1->pos.x = 100;
	client1->pos.y = 100;

	Unit triangleObjectEnemy = Unit(glm::vec3(0.4f, 0.1f, 0.2f));
	triangleObjectEnemy.setModel(&triangle);
	triangleObjectEnemy.setPosition(50, 50);
	objects.push_back(&triangleObjectEnemy);
	client2->pos.x = 100;
	client2->pos.y = 100;
	
	Text2D textPrinter;
	textPrinter.initText2D(textShaderID, "font.DDS", windowWidth, windowHeight);

	//Time variables
	double lastTime = glfwGetTime();

	GLuint ViewLocation = glGetUniformLocation(programID, "ViewMatrix");
	GLuint ProjectionLocation = glGetUniformLocation(programID, "ProjectionMatrix");

	while (glfwWindowShouldClose(glfwWindow) == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Calculate deltaTime
		double currentTime = glfwGetTime();
		deltaTime = (float)fabs(currentTime - lastTime);


		glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &RTSCamera::getInstance()->getViewMatrix()[0][0]);
		glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &RTSCamera::getInstance()->getProjectionMatrix()[0][0]);

		//Draw and Update all objects
		//Start with map (static)
		for (unsigned int i = 0; i < map.size(); i++)
		{
			map.at(i).draw();
		}
		// proceed with rest of objects.
		for (unsigned int i = 0; i < objects.size(); i++)
		{
			objects.at(i)->update(deltaTime);
			objects.at(i)->draw();	
		}
		
		
		//Switch program
 		glUseProgram(textShaderID);

		//write all text's you want.
		textPrinter.printText2D("This is a sample text!", 200, 200, 20);
		textPrinter.printText2D("Use WSAD to control the player.", 0, 0, 21);
		textPrinter.printText2D("Use the ARROWS to control the Camera.", 0, 40, 21);
		textPrinter.printText2D("Use the scroll or NUMPAD+/- to control the Zoom.", 0, 80, 21);

		//switch back
		glUseProgram(programID);

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
		
		handleEvents(triangleObject);
		handleEvents(triangleObjectEnemy);

		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		{
			RTSCamera::getInstance()->Zoom(1);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		{
			RTSCamera::getInstance()->Zoom(-1);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
		{
			RTSCamera::getInstance()->Move(0, 1);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			RTSCamera::getInstance()->Move(0, -1);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			RTSCamera::getInstance()->Move(-1, 0);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			RTSCamera::getInstance()->Move(1, 0);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			sendMove("down");
			Sleep(200);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			sendMove("up");
			Sleep(200);

		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			sendMove("right");
			Sleep(200);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		{	
			sendMove("left");
			Sleep(200);

		}

		triangleObjectEnemy.Object::setX(40 +(client2->pos.x + 100) * 2);
		triangleObject.Object::setX(40 +(client1->pos.x + 100) * 2);
		triangleObjectEnemy.Object::setY(40 + (client2->pos.y + 100) * 2);
		triangleObject.Object::setY(40 + (client1->pos.y + 100) * 2);
		//Set lastTime to currentTime!
		lastTime = currentTime;
	}
}

void myWindow::handleEvents(Unit &player) 
{
// 	int speed = 200;
// 	if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS) 
// 	{
// 		player.Object::incrementY(speed*deltaTime *-1);
// 	}
// 	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS) 
// 	{
// 		player.Object::incrementY(speed*deltaTime);
// 	}
// 	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS) 
// 	{
// 		player.Object::setX(client1->pos.x + 100);
// 		sendMove("right");
// 	}
// 	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS) 
// 	{
// 		player.Object::setX(client1->pos.x + 100);
// 		sendMove("left");
// 	}
	player.Object::UpdatePos();
}
