#include "myWindow.h"
#include "Text2D.h"
#include "RTSCamera.h"
#include "flags.h"
#include "AStar.h"
#include "tileType.h"
#include "Scout.h"
#include "EntityManager.h"
#include <time.h>
#include "BFSearch.h"
#include "DFSearch.h"
#include "Worker.h"
#include "Builder.h"
#include "Kilner.h"
#include "SettingsLoader.h"

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

	SettingsLoader* lod = SettingsLoader::Instance();
	test = Pathfinder::Instance();
	test->Load("karta.txt");

	mgr = EntityManager::Instance();
	int randSpwn = test->getStart().getIndex();
	mgr->addVisitedNode(randSpwn);

	for (int j = randSpwn - 101; j <= randSpwn - 99; j++)
	{
		if (!(test->getGraph().getNode(j).getType() == mountain || test->getGraph().getNode(j).getType() == water))
		{
			mgr->addVisitedNode(j);
		}

	}
	for (int k = randSpwn - 1; k <= randSpwn + 1; k++)
	{
		if (!(test->getGraph().getNode(k).getType() == mountain || test->getGraph().getNode(k).getType() == water))
		{
			mgr->addVisitedNode(k);
		}

	}
	for (int l = randSpwn + 99; l <= randSpwn + 101; l++)
	{
		if (!(test->getGraph().getNode(l).getType() == mountain || test->getGraph().getNode(l).getType() == water))
		{
			mgr->addVisitedNode(l);
		}

	}
	//Creates graph
	teeest = test->getGraph();

	for (int i = 0; i < 50; i++)
	{
		Worker* guy2 = new Worker(glm::vec2(0.0f, 0.0f), 2.0f, 2.0f, lod->getentMaxVelocity());
		mgr->RegisterEntity(guy2);
		guy2->getStateMachine()->ChangeStateNotWorking(SearchTree::Instance());
	}


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
		Model treePoint = shapeGenerator::square(1, 1, programID);

		if (teeest.getNode(i).getFlag() == walkable)
		{


			if (teeest.getNode(i).getType() == tree)
 			{
				for (int j = 0; j < teeest.getNode(i).getNodeTiles().size(); j++)
				{
					Unit treeobject = Unit(glm::vec3(0.0f, 1.0f, 0.0f));
					glm::vec2 tempPos = teeest.getNode(i).getPos();
					int newx, newy;

 					if (teeest.getNode(i).getNodeTiles()[j] == 1)
 					{
 						if (j == 0)
 						{
 							treeobject.setModel(&treePoint);
 							newx = tempPos.x * tileSize;
 							newy = tempPos.y * tileSize;
 
 						}
 						else if (j == 1 || j == 2)
 						{
 
 							treeobject.setModel(&treePoint);
 							newx = tempPos.x * tileSize + j * treeOff;
 							newy = tempPos.y * tileSize;
 						}
 						else if (j == 3 || j == 6)
 						{
 							
 							treeobject.setModel(&treePoint);
 							newx = tempPos.x * tileSize;
 							newy = tempPos.y * tileSize + (j *0.33) * treeOff;
 						}
 						else if (j ==  4 || j == 7)
 						{
 							if (j == 4 )
 							{
 								treeobject.setModel(&treePoint);
 								newx = tempPos.x * tileSize + 1 * treeOff;
 								newy = tempPos.y * tileSize + 1 * treeOff;
 							}
 							else
 							{
 								treeobject.setModel(&treePoint);
 								newx = tempPos.x * tileSize + 1 * treeOff;
 								newy = tempPos.y * tileSize + 2 * treeOff;
 							}
 
 						}
 						else if (j == 5 || j == 8)
 						{
 							if (j == 5)
 							{
 								treeobject.setModel(&treePoint);
 								newx = tempPos.x * tileSize + 2 * treeOff;
 								newy = tempPos.y * tileSize + 1 * treeOff;
 							}
 							else
 							{
 								treeobject.setModel(&treePoint);
 								newx = tempPos.x * tileSize + 2 * treeOff;
 								newy = tempPos.y * tileSize + 2 * treeOff;
 							}
 
 						}
 						treeobject.setPosition(newx, newy);
 						mapTrees.push_back(treeobject);

 						Unit treeobject = Unit(glm::vec3(1.0f, 1.0f, 1.0f));
					}

				}
				Unit squariObject = Unit(glm::vec3(0.5f, 0.5f, 0.5f)*0.5f);
				squariObject.setModel(&squari);
				squariObject.setPosition(0 + x * (tileSize + 0), 0 + y * (tileSize + 0));
				map.push_back(squariObject);
			}
			else if (teeest.getNode(i).getType() == marsh)
			{
				Unit squariObject = Unit(glm::vec3(0.3f, 0.3f, 0.0f)*0.5f);
				squariObject.setModel(&squari);
				squariObject.setPosition(0 + x * (tileSize + 0), 0 + y * (tileSize + 0));
				map.push_back(squariObject);
			}
			else
			{
				Unit squariObject = Unit(glm::vec3(0.5f, 0.5f, 0.5f)*0.5f);
				squariObject.setModel(&squari);
				squariObject.setPosition(0 + x * (tileSize + 0), 0 + y * (tileSize + 0));
				map.push_back(squariObject);
			}
		}
		if (teeest.getNode(i).getFlag() == unwalkable)
		{


			if (teeest.getNode(i).getType() == water)
			{
				Unit squariObject = Unit(glm::vec3(0.0f, 0.0f, 0.5f));
				squariObject.setModel(&squari);
				squariObject.setPosition(0 + x * (tileSize + 0), 0 + y * (tileSize + 0));
				map.push_back(squariObject);
			}
			else
			{
				Unit squariObject = Unit(glm::vec3(1.0f, 1.0f, 1.0f));
				squariObject.setModel(&squari);
				squariObject.setPosition(0 + x * (tileSize + 0), 0 + y * (tileSize + 0));
				map.push_back(squariObject);
			}

		}
		if (teeest.getNode(i).getFlag() == goal)
		{
			Unit squariObject = Unit(glm::vec3(0.0f, 1.0f, 0.0f)*0.5f);
			squariObject.setModel(&squari);
			squariObject.setPosition(0 + x * (tileSize + 0), 0 + y * (tileSize + 0));
			map.push_back(squariObject);
		}
		if (teeest.getNode(i).getFlag() == start)
		{
			Unit squariObject = Unit(glm::vec3(1.0f, 0.0f, 0.0f)*0.5f);
			squariObject.setModel(&squari);
			squariObject.setPosition(0 + x * (tileSize + 0), 0 + y * (tileSize + 0));
			map.push_back(squariObject);
		}
		x++;
	}
	//Time variables
	double lastTime = glfwGetTime();

	GLuint ViewLocation = glGetUniformLocation(programID, "ViewMatrix");
	GLuint ProjectionLocation = glGetUniformLocation(programID, "ProjectionMatrix");

	Model squari2 = shapeGenerator::square(3, 3, programID);

	for (int i = 0; i < mgr->getEntityMap().size(); i++)
	{
		mgr->GetEntity(i)->Update(deltaTime);

		glm::vec2 tempPosFrom = mgr->GetEntity(i)->Pos();
	

		Unit* squariObject = new Unit(glm::vec3(1.0f, 0.0f, 1.0f));

		mgr->GetEntity(i)->setSprite(squariObject);
		squariObject->setModel(&squari2);
		squariObject->setPosition((tempPosFrom.x * tileSize) + 2, (tempPosFrom.y * tileSize) + 2);
		objects.push_back(squariObject);

	}

	Text2D textPrinter;
	textPrinter.initText2D(textShaderID, "font.DDS", windowWidth, windowHeight);

	while (glfwWindowShouldClose(glfwWindow) == 0)
	{
		currentTime2 = glfwGetTime();

		fps++;

		char f[24];



		if (currentTime2 - oldTime >= 1)
		{
			oldTime = currentTime2;
			//std::cout << "FPS: " << fps << std::endl;
			sprintf(f, "%d", fps);
			fps = 0;
		}

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
		for (unsigned int i = 0; i < mapTrees.size(); i++)
		{
			
			mapTrees.at(i).draw();
		}
		// proceed with rest of objects.
		for (unsigned int i = 0; i < objects.size(); i++)
		{
			objects.at(i)->update(deltaTime);
			objects.at(i)->draw();	
		}


		glUseProgram(textShaderID);


		textPrinter.printText2D(f, 200, 200, 20);
		textPrinter.printText2D("Fps", 235, 200, 20);

		char c[24];
		sprintf(c, "%d", mgr->getCharcoal());
		textPrinter.printText2D(c, 200, 220, 20);
		
		//Switch program
 		glUseProgram(textShaderID);

		//switch back
		glUseProgram(programID);

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
		
// 		handleEvents(triangleObject);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		for (int i = 0; i < mgr->getEntityMap().size(); i++)
		{
			double currentTime = glfwGetTime();
			deltaTime = (float)fabs(currentTime - lastTime);
			mgr->GetEntity(i)->Update(deltaTime);
			glm::vec2 tempPosFrom = mgr->GetEntity(i)->Pos();
			mgr->GetEntity(i)->getSprite();
			mgr->GetEntity(i)->getSprite()->setPosition((tempPosFrom.x * tileSize) + 2, (tempPosFrom.y * tileSize) + 2);

			


		}


		


// 
// 		guy->Update(deltaTime);
// 		glm::vec2 tempPosFrom = guy->Pos();
// 
// 		Model squari0 = shapeGenerator::square(5, 5, programID);
// 		Unit squariObject = Unit(glm::vec3(1.0f, 0.0f, 1.0f));
// 		squariObject.setModel(&squari0);
// 		squariObject.setPosition((tempPosFrom.x * tileSize) + 2, (tempPosFrom.y * tileSize) + 2);
// 		objects.push_back(&squariObject);

// 		guy2->Update(deltaTime);
//  		glm::vec2 tempPosFrom2 = guy2->getLocationNode().getPos();
//  
//  		Model squari2 = shapeGenerator::square(5, 5, programID);
//  		Unit squariObject2 = Unit(glm::vec3(1.0f, 1.0f, 0.0f));
//  		squariObject2.setModel(&squari2);
// 		squariObject2.setPosition((tempPosFrom2.x * tileSize) + 2, (tempPosFrom2.y * tileSize) + 2);
// 		objects.push_back(&squariObject2);

// 		
// 		guy3->Update();
// 		glm::vec2 tempPosFrom3 = guy3->getLocationNode().getPos();
// 
// 		Model squari3 = shapeGenerator::square(5, 5, programID);
// 		Unit squariObject3 = Unit(glm::vec3(0.0f, 1.0f, 1.0f));
// 		squariObject3.setModel(&squari3);
// 		squariObject3.setPosition((tempPosFrom3.x * tileSize) + 2, (tempPosFrom3.y * tileSize) + 2);
// 		objects.push_back(&squariObject3);
// 
// 		guy4->Update();
// 		glm::vec2 tempPosFrom4 = guy4->getLocationNode().getPos();
// 
// 		Model squari4 = shapeGenerator::square(5, 5, programID);
// 		Unit squariObject4 = Unit(glm::vec3(0.0f, 1.0f, 1.0f));
// 		squariObject4.setModel(&squari4);
// 		squariObject4.setPosition((tempPosFrom4.x * tileSize) + 2, (tempPosFrom4.y * tileSize) + 2);
// 		objects.push_back(&squariObject4);
// 
// 
// 		guy5->Update();
// 		glm::vec2 tempPosFrom5 = guy5->getLocationNode().getPos();
// 
// 		Model squari5 = shapeGenerator::square(5, 5, programID);
// 		Unit squariObject5 = Unit(glm::vec3(0.0f, 1.0f, 1.0f));
// 		squariObject5.setModel(&squari5);
// 		squariObject5.setPosition((tempPosFrom5.x * tileSize) + 2, (tempPosFrom5.y * tileSize) + 2);
// 		objects.push_back(&squariObject5);


		std::list<int>* visNodeIndexList = &mgr->getVisitedNodesDraw();
		for (std::list<int>::iterator it = visNodeIndexList->begin(); it != visNodeIndexList->end(); it++)
		{
			if (teeest.getNode(*it).getFlag() == walkable)
			{
				if (teeest.getNode(*it).getType() == tree)
				{
					map.at(*it).setColor(glm::vec3(0.5f, 0.5f, 0.5f));

				}
				else if (teeest.getNode(*it).getType() == marsh)
				{
					map.at(*it).setColor(glm::vec3(0.3f, 0.3f, 0.0f));
				}
				else if (teeest.getNode(*it).getType() == charcoalKiln)
				{
					map.at(*it).setColor(glm::vec3(0.0f, 0.0f, 0.0f));
				}
				else
				{
					map.at(*it).setColor(glm::vec3(0.5f, 0.5f, 0.5f));
				}
			}

			


		}
		visNodeIndexList->clear();

		for (int i = 0; i < mgr->getBuildingPositions().size(); i++)
		{
			map.at(mgr->getBuildingPositions()[i]).setColor(glm::vec3(0.0f, 0.0f, 0.0f));
		}

		

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
	int speed = 200;
	if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS) {
		player.Object::incrementY(speed*deltaTime *-1);
	}if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS) {
		player.Object::incrementY(speed*deltaTime);
	}if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS) {
		player.Object::incrementX(speed*deltaTime);
	}if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS) {
		player.Object::incrementX(speed*deltaTime*-1);
	}
	player.Object::UpdatePos();
}
