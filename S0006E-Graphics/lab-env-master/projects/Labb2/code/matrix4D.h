#include <iostream>
#include "vector4D.h"
#include "vector3D.h"
#pragma once


#define PI 3.14159265

class matrix4D
{
public:

//Nummer av columner och rader i matrixen som sätts till 3 istället för att skriva -1 överallt där "w" inte ska användas
int COLUMN = 4;
int ROW = 4;

//2D array
float values2D[4][4];


inline matrix4D(void)
{


	

	//Sätter sista värdet till 1 och alla andra värden i rad 4 och column 4 till 0
	this->values2D[0][0] = 1;
	this->values2D[0][1] = 0;
	this->values2D[0][2] = 0;
	this->values2D[0][3] = 0;

	this->values2D[1][0] = 0;
	this->values2D[1][1] = 1;
	this->values2D[1][2] = 0;
	this->values2D[1][3] = 0;

	this->values2D[2][0] = 0;
	this->values2D[2][1] = 0;
	this->values2D[2][2] = 1;
	this->values2D[2][3] = 0;

	this->values2D[3][0] = 0;
	this->values2D[3][1] = 0;
	this->values2D[3][2] = 0;
	this->values2D[3][3] = 1;
}

inline ~matrix4D(void)
{
	
}


// matrix4D LookAtRH(vector3D eye, vector3D target, vector3D up)
// {
// 	vector3D zaxis = (eye - target).normalize();    // The "forward" vector.
// 	vector3D xaxis = (up.cross(zaxis)).normalize();// The "right" vector.
// 	vector3D yaxis = zaxis.cross(xaxis);     // The "up" vector.
// 
// 	// Create a 4x4 orientation matrix from the right, up, and forward vectors
// 	// This is transposed which is equivalent to performing an inverse 
// 	// if the matrix is orthonormalized (in this case, it is).
// 
// 	matrix4D orientation;
// 
// 		orientation[0][0] = xaxis[0];
// 		orientation[0][1] = xaxis[1];
// 		orientation[0][2] = xaxis[2];
// 		orientation[0][3] = 0;
// 
// 		orientation[1][0] = yaxis[0];
// 		orientation[1][1] = yaxis[1];
// 		orientation[1][2] = yaxis[2];
// 		orientation[1][3] = 0;
// 
// 		orientation[2][0] = zaxis[0];
// 		orientation[2][1] = zaxis[1];
// 		orientation[2][2] = zaxis[2];
// 		orientation[2][3] = 0;
// 
// 		orientation[3][0] = 0;
// 		orientation[3][1] = 0;
// 		orientation[3][2] = 0;
// 		orientation[3][3] = 1;
// 
// 
// 	// Create a 4x4 translation matrix.
// 	// The eye position is negated which is equivalent
// 	// to the inverse of the translation matrix. 
// 	// T(v)^-1 == T(-v)
// 		matrix4D translation;
// 
// 		values2D[3][0] = -(eye[0]);
// 		values2D[3][1] = -(eye[1]);
// 		values2D[3][2] = -(eye[2]);
// 	
// 
// 	// Combine the orientation and translation to compute 
// 	// the final view matrix
// 	return (orientation * translation);
// }


//asd
inline matrix4D View(vector3D eye, vector3D target, vector3D up)
{
	vector3D x, y, z;
	vector3D vect(0,0,0);





	z = target - eye;
	z = z.normalize(z);
	x = up.cross(z);
	x = x.normalize(x);
	y = z.cross(x);


	matrix4D viewm;
	viewm[0][0] = x[0];
	viewm[0][1] = x[1];
	viewm[0][2] = x[2];
	viewm[0][3] = -x.dott(eye);

	viewm[1][0] = y[0];
	viewm[1][1] = y[1];
	viewm[1][2] = y[2];
	viewm[1][3] = -y.dott(eye);

	viewm[2][0] = z[0];
	viewm[2][1] = z[1];
	viewm[2][2] = z[2];
	viewm[2][3] = -z.dott(eye);
		 
	viewm[3][0] = 0.0f;
	viewm[3][1] = 0.0f;
	viewm[3][2] = 0.0f;
	viewm[3][3] = 1.0f;

	return viewm;
}


matrix4D LookAtRH(vector3D eye, vector3D target, vector3D up)
{
	vector3D zaxis = (eye - target);
	zaxis.normalize();
	vector3D xaxis = (up.cross(zaxis));// The "right" vector.
	xaxis.normalize();
	vector3D yaxis = zaxis.cross(xaxis);    // The "forward" vector.
	
     // The "up" vector.

	// Create a 4x4 view matrix from the right, up, forward and eye position vectors
	matrix4D viewMatrix;
	viewMatrix[0][0] = xaxis[0];
	viewMatrix[1][0] = xaxis[1];
	viewMatrix[2][0] = xaxis[2];
	viewMatrix[3][0] = -(xaxis.dott(eye));
	
	viewMatrix[0][1] = yaxis[0];
	viewMatrix[1][1] = yaxis[1];
	viewMatrix[2][1] = yaxis[2];
	viewMatrix[3][1] = -(yaxis.dott(eye));
	
	viewMatrix[0][2] = zaxis[1];
	viewMatrix[1][2] = zaxis[2];
	viewMatrix[2][2] = zaxis[3];
	viewMatrix[3][2] = -(zaxis.dott(eye));
	
	viewMatrix[0][3] = 0.0f;
	viewMatrix[1][3] = 0.0f;
	viewMatrix[2][3] = 0.0f;
	viewMatrix[3][3] = 1.0f;

	


	return viewMatrix;
}


matrix4D setPerspective(float fovY, float aspect, float zNear, float zFar)
{
	matrix4D temp;
	

	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf((PI / 180.0f)*(fovY * 0.5f));

	temp[0][0] = 1.0f / (tanHalfFOV * aspect);
	temp[0][1] = 0.0f;
	temp[0][2] = 0.0f;
	temp[0][3] = 0.0f;
	
	temp[1][0] = 0.0f;
	temp[1][1] = 1.0f / tanHalfFOV;
	temp[1][2] = 0.0f;
	temp[1][3] = 0.0f;
	
	temp[2][0] = 0.0f;
	temp[2][1] = 0.0f;
	temp[2][2] = ((-zNear) - zFar) / zRange;
	temp[2][3] = 2.0f * zFar * zNear / zRange;
	
	temp[3][0] = 0.0f;
	temp[3][1] = 0.0f;
	temp[3][2] = 1.0f;
	temp[3][3] = 0.0f;


	// params: left, right, bottom, top, near, far
	return temp;
}

inline float* get()
{
	
	return (float*)values2D;
}

inline matrix4D scale(float x, float y, float z)
{
	matrix4D temp;
	temp[0][0] = x;
	temp[1][1] = y;
	temp[2][2] = z;
	return temp;
}

inline matrix4D transform(float x, float y, float z)
{
	
	values2D[0][3] = x;
	values2D[1][3] = y;
	values2D[2][3] = z;
	return (*this);
}

//Roatation runt x axeln i "r" grader
inline matrix4D rot_x(float r)
{
	matrix4D x;
	float c = cos(r * PI / 180.0f);
	float s = sin(r * PI / 180.0f);

	x.values2D[0][0] = 1;
	x.values2D[0][1] = 0;
	x.values2D[0][2] = 0;
	x.values2D[1][0] = 0;
	x.values2D[1][1] = c;
	x.values2D[1][2] = -s;
	x.values2D[2][0] = 0;
	x.values2D[2][1] = s;
	x.values2D[2][2] = c;


	return  x;
}
///Roatation runt y axeln i "r" grader
inline matrix4D rot_y(float r)
{
	matrix4D y;
	float c = cos(r * PI / 180.0f);
	float s = sin(r * PI / 180.0f);

	y.values2D[0][0] = c;
	y.values2D[0][1] = 0;
	y.values2D[0][2] = s;
	y.values2D[1][0] = 0;
	y.values2D[1][1] = 1;
	y.values2D[1][2] = 0;
	y.values2D[2][0] = -s;
	y.values2D[2][1] = 0;
	y.values2D[2][2] = c;

	return  y;
}
//Roatation runt z axeln i "r" grader
inline matrix4D rot_z(float r)
{
	matrix4D z;
	float c = cos(r * PI / 180.0f);
	float s = sin(r * PI / 180.0f);

	z.values2D[0][0] = c;
	z.values2D[0][1] = -s;
	z.values2D[0][2] = 0;
	z.values2D[1][0] = s;
	z.values2D[1][1] = c;
	z.values2D[1][2] = 0;
	z.values2D[2][0] = 0;
	z.values2D[2][1] = 0;
	z.values2D[2][2] = 1;

	return z;
}
//Roatation runt en vector i "r" grader
inline matrix4D rot_v(const vector4D v, float r)
{
	matrix4D temp;

	float c = cos(r * PI / 180.0f);
	float s = sin(r * PI / 180.0f);
	float x = v.Varray[0];
	float y = v.Varray[1];
	float z = v.Varray[2];

	temp.values2D[0][0] = pow(x, 2) + (1 - pow(x, 2)) * c;
	temp.values2D[0][1] = x * y * (1 - c) - x * s;
	temp.values2D[0][2] = x * z * (1 - c) + y * s;
	temp.values2D[1][0] = x * y * (1 - c) + z * s;
	temp.values2D[1][1] = pow(y, 2) + (1 - pow(y, 2)) * c;
	temp.values2D[1][2] = x * z * (1 - c) - x * s;
	temp.values2D[2][0] = x * z * (1 - c) - y * s;
	temp.values2D[2][1] = y * z * (1 - c) + x * s;
	temp.values2D[2][2] = pow(z, 2) + (1 - pow(z, 2)) * c;

	return temp;

}
//Gångrar en matris med en matirs
inline matrix4D operator*(const matrix4D& m)
{
	matrix4D temp(*this);
	matrix4D m2;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m2.values2D[i][j] = 0;

			for (int k = 0; k < 4; k++)
			{
				m2.values2D[i][j] += temp.values2D[i][k] * m.values2D[k][j];
			}
		}
	}
	return m2;
}
//Gångrar en matris med en vector
inline vector4D& operator*(const vector4D& v)
{
	vector4D temp;
	temp.setValues(0);

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			temp.Varray[i] += (this->values2D[i][j] * v.Varray[i]);
		}
	}
	return temp;
}
//Tillåter utskrivning av en matris
inline friend std::ostream& operator<<(std::ostream& os,const matrix4D& m)
{
	for (int i = 0; i<m.ROW+1; i++)
	{
		for (int j = 0; j<m.COLUMN+1; j++)
		{
			os << m.values2D[i][j] << "\t";
		}
		os << std::endl;
	}
	return os;
}
//Sätter alla värden i matrisen till "v"
inline void setValues(int v)
{
	for (int i = 0; i<ROW; i++)
	{
		for (int j = 0; j<COLUMN; j++)
		{
			values2D[i][j] = v;
		}
	}
}
//skickar tillbaka transponatet av en matrix
inline void operator~()
{
	

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			values2D[j][i] = values2D[i][j];
		}
	}
	
}


float* operator[](int n)
{

	return this->values2D[n];

}

//Ger determinanten av en matrix (används för att räkna ut inversen av en matris)
inline float det(void)
{
	double a = this->values2D[0][0] * (this->values2D[1][1] * this->values2D[2][2] - this->values2D[2][1] * this->values2D[1][2]);
	double b = this->values2D[0][1] * (this->values2D[1][0] * this->values2D[2][2] - this->values2D[2][0] * this->values2D[1][2]);
	double c = this->values2D[0][2] * (this->values2D[1][0] * this->values2D[2][1] - this->values2D[2][0] * this->values2D[1][1]);

	return a - b + c;
}
//Ger adjugatet av en matrix (används för att räkna ut inversen av en matris)
inline matrix4D adj(void)
{
	matrix4D temp;

	temp.values2D[0][0] =	this->values2D[1][1] * this->values2D[2][2] - this->values2D[2][1] * this->values2D[1][2];
	temp.values2D[0][1] = -(this->values2D[0][1] * this->values2D[2][2] - this->values2D[2][1] * this->values2D[0][2]);
	temp.values2D[0][2] =	this->values2D[0][1] * this->values2D[2][2] - this->values2D[2][1] * this->values2D[0][2];
	temp.values2D[1][0] = -(this->values2D[1][0] * this->values2D[2][2] - this->values2D[2][0] * this->values2D[1][2]);
	temp.values2D[1][1] =	this->values2D[0][0] * this->values2D[2][2] - this->values2D[2][0] * this->values2D[0][2];
	temp.values2D[1][2] = -(this->values2D[0][0] * this->values2D[2][1] - this->values2D[2][0] * this->values2D[0][1]);
	temp.values2D[2][0] =	this->values2D[0][1] * this->values2D[1][2] - this->values2D[1][1] * this->values2D[0][2];
	temp.values2D[2][1] = -(this->values2D[0][0] * this->values2D[0][2] - this->values2D[1][0] * this->values2D[1][2]);
	temp.values2D[2][2] =	this->values2D[0][0] * this->values2D[1][1] - this->values2D[1][0] * this->values2D[0][1];

	return temp;
}
//Skickar tillbaka inversen av en matrix
inline matrix4D inv()
{
	matrix4D temp;
	matrix4D ajd = this->adj();

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{

			temp.values2D[i][j] = (ajd.values2D[i][j] / this->det());
		}
	}
	return temp;
}




};

