// NPLabb1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "matrix3D.h"
#include "matrix4D.h"



int _tmain(int argc, _TCHAR* argv[])
{

	vector3D V1;
	vector3D V2;
	matrix3D M1;
	matrix4D M2;

	M1.setValues(1);


	
	V1[0] = 1;
	V1[1] = 1;
	V1[2] = 1;


	//v2Varray[0] = 4;
	//V2.Varray[1] = 3;
	
	//V2.Varray[2] = 2;




	M1.values2D[0][0] = 1;
	M1.values2D[0][1] = 0;
	M1.values2D[0][2] = 1;
	M1.values2D[1][0] = 1;
	M1.values2D[1][1] = 1;
	M1.values2D[1][2] = 0;
	M1.values2D[2][0] = 0;
	M1.values2D[2][1] = 1;
	M1.values2D[2][2] = 1;


	
	std::cout << M2;
	std::cout << M2.inv();
	//std::cout << M1;
	//std::cout << M1.inv();

	//std::cout << M1;
	//std::cout << M1.inv();

	//V1+V2;


	

	std::system("PAUSE");
}

