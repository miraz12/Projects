#include "myWindow.h"
#include "Text2D.h"
#include "RTSCamera.h"
#include "flags.h"
#include "BFSearch.h"
#include "DFSearch.h"
#include "AStar.h"
#include "ShaggySearch.h"


//Standard functions ****************************************

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

	///////////////////////////////////////////////////////ändra till Map1 Map2 eller Map3//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Graph test
	test = Pathfinder::Instance();
	test->Load("Map3.txt");

	installShaders();
	//Use installed shader
	glUseProgram(programID);
	//Start running game loop
	gameLoop();
}

void myWindow::installShaders() {
	programID = LoadShaders("vertexShader.gsl", "fragmentShader.gsl");
	textShaderID = LoadShaders("TextVertexShader.gsl", "TextFragmentShader.gsl");
	//debugProgramID = LoadShaders("debugVertexShader.glsl", "debugFragmentShader.glsl");
}

void myWindow::gameLoop() 
{
	//Text2D textPrinter;
	//textPrinter.initText2D(textShaderID, "font.DDS", windowWidth, windowHeight);
	// 
	//Creates graph
	SparseGraph<NavNode, NavEdge> teeest = test->getGraph();

	//Time variables for calculationg alg time
	double lastTimeAlg = glfwGetTime();

	///////////////////////////////////////////////////////Komentera ut eller in den algoritm som ska användas////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Search algorithms
	//BFSearch search(test.getGraph(), test.getStart());
	//DFSearch search(test.getGraph(), test.getStart());
	AStar search(teeest, test->getStart(), test->getGoalNode());
	//ShaggySearch search(test.getGraph(), test.getStart(), test.getGoalNode());

	//Calculate and print deltaTime
	double currentTime = glfwGetTime();
	deltaTime = fabs(currentTime - lastTimeAlg);
	std::cout << "\n" << deltaTime << "\n";
	int y = 0;
	int x = 0;
	for (int i = 0; i < teeest.getNumNodes(); i++)
	{
		if (x == test->getGraphWidth())
		{
			x = 0;
			y++;
		}

		Model squari = shapeGenerator::square(tileSize, tileSize, programID);

		if (teeest.getNode(i).getFlag() == walkable)
		{

			Unit squariObject = Unit(glm::vec3(0.5f, 0.5f, 0.5f));
			squariObject.setModel(&squari);
			squariObject.setPosition(0 + x * (tileSize + 0), 0 + y * (tileSize + 0));
			objects.push_back(squariObject);
		}
		if (teeest.getNode(i).getFlag() == unwalkable)
		{
			Unit squariObject = Unit(glm::vec3(1.0f, 1.0f, 1.0f));
			squariObject.setModel(&squari);
			squariObject.setPosition(0 + x * (tileSize + 0), 0 + y * (tileSize + 0));
			objects.push_back(squariObject);
		}
		if (teeest.getNode(i).getFlag() == goal)
		{
			Unit squariObject = Unit(glm::vec3(0.0f, 1.0f, 0.0f));
			squariObject.setModel(&squari);
			squariObject.setPosition(0 + x * (tileSize + 0), 0 + y * (tileSize + 0));
			objects.push_back(squariObject);
		}
		if (teeest.getNode(i).getFlag() == start)
		{
			Unit squariObject = Unit(glm::vec3(1.0f, 0.0f, 0.0f));
			squariObject.setModel(&squari);
			squariObject.setPosition(0 + x * (tileSize + 0), 0 + y * (tileSize + 0));
			objects.push_back(squariObject);
		}
		x++;
	}

	std::vector<int> p = search.GetPathToTarget();

	for (unsigned int i = 0; i < p.size(); i++)
	{
		glm::vec2 tempPosFrom = teeest.getNode(p[i]).getPos();

		Model squari = shapeGenerator::square(5, 5, programID);

		Unit squariObject = Unit(glm::vec3(0.0f, 0.0f, 1.0f));
		squariObject.setModel(&squari);
		squariObject.setPosition((tempPosFrom.x * tileSize) + 12, (tempPosFrom.y * tileSize) + 12);
		objects.push_back(squariObject);
	}

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
			objects.at(i).update(deltaTime);
			objects.at(i).draw();	
		}
		
		
		//Switch program
 		glUseProgram(textShaderID);

		//write all text's you want.
		//textPrinter.printText2D("This is a sample text!", 200, 200, 20);
		//textPrinter.printText2D("Use WSAD to control the player.", 0, 0, 21);
		//textPrinter.printText2D("Use the ARROWS to control the Camera.", 0, 40, 21);
		//textPrinter.printText2D("Use the scroll or NUMPAD+/- to control the Zoom.", 0, 80, 21);

		//switch back
		glUseProgram(programID);

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
		
		//handleEvents(triangleObject);

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

		//Set lastTime to currentTime!
		lastTime = currentTime;
	}
}

void myWindow::handleEvents(Unit &player) 
{
	//int speed = 200;
	//if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS) {
	//	player.Object::incrementY(speed*deltaTime *-1);
	//}if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS) {
	//	player.Object::incrementY(speed*deltaTime);
	//}if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS) {
	//	player.Object::incrementX(speed*deltaTime);
	//}if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS) {
	//	player.Object::incrementX(speed*deltaTime*-1);
	//}
	//player.Object::UpdatePos();
}
