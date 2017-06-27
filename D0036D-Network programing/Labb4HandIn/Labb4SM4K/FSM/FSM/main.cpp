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
#include <thread>
#include "RC4.h"

void main(int argc, char* argv[]) { 

	//Window object
	myWindow window = myWindow();

	//Initialize GL, must initialize, otherwise abort!
	assert(window.initializeGL() == 0);

	//Create a window, abort if not.
	assert(window.createWindow() == 0);

	//Window established, call custom functions

	//Initialize scene



	window.initializeNetwork();

	NetworkListener* nList = new NetworkListener(window.client1, window.client2, window.ConnectSocket, window.getRc4In(), window.getRc4Out());
	std::thread t1(&NetworkListener::listener, nList);

	//window.join();

	window.initializeScene();





}