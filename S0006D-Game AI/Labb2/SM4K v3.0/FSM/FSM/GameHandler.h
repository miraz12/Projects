#pragma once
#include "Map.h"
#include "GL\glew.h"
//#include "Object.h"

class Object;

/************************************************************************/
/* SINGLETON 

Keeps track of the game!

*/
/************************************************************************/

class GameHandler
{
private:
	Map map;

	//Decides window Width and Height
	//This makes the dimensions easily accessible
	//Note:
	//If you change these settings you need to change the settings in the textshader too.
	//TODO: Change so that textshader loads this automatically
	const float windowWidth = 1100; 
	const float windowHeight = 1100;

	GameHandler();

public:
	static GameHandler* getInstance()
	{
		static GameHandler instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	GameHandler(GameHandler const&) = delete;
	void operator=(GameHandler const&) = delete;
	
	bool setMap(char* mapFile);
	Map getMap() const;

	const int tileSize = 40;

	float getWindowWidth() const { return windowWidth; }
	float getWindowHeight() const { return windowHeight; }
};
