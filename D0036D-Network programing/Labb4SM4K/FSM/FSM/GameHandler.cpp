#pragma once
#include "GameHandler.h"
#include "shapeGenerators.hpp"
#include "Unit.h"

GameHandler::GameHandler()
{
}

/************************************************************************/
/* setMap Function
Very ugly and should be rewritten.

returns true if map has been loaded.
*/
/************************************************************************/
bool GameHandler::setMap(char* file)
{
	if (map.filename != NULL)
	{
		printf("Map has already been loaded! You can't switch map after loading one already!\n");
		return true;
	}
	// set mapname
	map.filename = file;

	// setup map height and width
	FILE * f = fopen(file, "r");
	if (file == NULL){
		printf("Impossible to open the file !\n");
		return false;
	}

	int ch;

	unsigned int x = 0;
	unsigned int y = 0;

	while (EOF != (ch = getc(f)))
	{
		Tile temptile;
		if (ch != '\n')
		{
			if (ch == 'X')
			{
				temptile.passable = false;
				temptile.start = false;
				temptile.goal = false;
			}
			else if (ch == '0')
			{
				temptile.passable = true;
				temptile.start = false;
				temptile.goal = false;
			}
			else if (ch == 'S')
			{
				temptile.passable = true;
				temptile.start = true;
				temptile.goal = false;
			}
			else if (ch == 'G')
			{
				temptile.passable = true;
				temptile.start = false;
				temptile.goal = true;
			}
			else
			{
				printf("Cannot process mapfile! Unexpected character!\n");
			}

			this->map.setTile(x, y, temptile);
			x++;
		}
		else
		{
			y++;
			x = 0;
		}
	}
	
	y++; // Add one to height since it does not count for the last line

	map.height = y;
	map.width = x;

	map.mapTrim(); //ALWAYS CALL THIS AFTER SETTING UP MAP

	//printf("Map is %u x %u\n", map.height, map.width);

	return true;
}

Map GameHandler::getMap() const
{
	return map;
}

