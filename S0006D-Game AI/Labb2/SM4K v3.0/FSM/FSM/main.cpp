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

void main(int argc, char* argv[]) { 

	//Window object
	myWindow window = myWindow();

	//Initialize GL, must initialize, otherwise abort!
	assert(window.initializeGL() == 0);

	//Create a window, abort if not.
	assert(window.createWindow() == 0);

	//Window established, call custom functions

	//Initialize scene
	window.initializeScene();

}