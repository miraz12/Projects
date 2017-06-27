#pragma once

//Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//Window class
#include "myWindow.h"

#include <iostream>

#include <stdio.h>
#include <math.h>

#include "SparseGraph.h"
#include "NavNode.h"
#include "NavEdge.h"
#include "Pathfinder.h"

void main(int argc, char* argv[]) { 

	//Window object 
	myWindow window = myWindow(1024.0f, 768.0f);

	//Initialize GL, must initialize, otherwise abort!
	assert(window.initializeGL() == 0);

	//Create a window, abort if not.
	assert(window.createWindow() == 0);

	//Window established, call custom functions

	//Initialize scene
	window.initializeScene();

}