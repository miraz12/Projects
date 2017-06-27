#include "config.h"
#include "Graphics.h"
#include <iostream>



int
main(int argc, const char** argv)
{

	//matrix4D n = matrix4D::matrix4D();
	//matrix4D n2 = matrix4D::matrix4D();


	//n3 = n * n2;
	//std::cout << n3;

	Labb2::Graphics app;
	if (app.Open())
	{
	app.Run();
	app.Close();
	}
	app.Exit();
	

}