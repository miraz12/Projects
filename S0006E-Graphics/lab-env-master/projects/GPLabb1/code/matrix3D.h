#include "stdafx.h"
#include <iostream>
#include "vector3D.h"

#define PI 3.14159265

class matrix3D
{
public:

int COLUMN = 3;
int ROW = 3;

//2D array
float** values2D;

matrix3D(void)
{
	this->values2D = new float*[ROW];
	for (int i = 0; i < ROW; i++)
	{
		this->values2D[i] = new float[COLUMN];
	}
}

~matrix3D(void)
{

}
//Roatation runt x axeln i "r" grader
inline matrix3D rot_x(float r )
{
	matrix3D x ;
	float c = cos(r * PI / 180.0);
	float s = sin(r * PI / 180.0);

	x.values2D[0][0] = 1;
	x.values2D[0][1] = 0;
	x.values2D[0][2] = 0;
	x.values2D[1][0] = 0;
	x.values2D[1][1] = c;
	x.values2D[1][2] = -s;
	x.values2D[2][0] = 0;
	x.values2D[2][1] = s;
	x.values2D[2][2] = c;

	return ((*this) * x);
}
//Roatation runt y axeln i "r" grader
inline matrix3D rot_y(float r)
{
	matrix3D y;
	float c = cos(r * PI / 180.0);
	float s = sin(r * PI / 180.0);

	y.values2D[0][0] = c;
	y.values2D[0][1] = 0;
	y.values2D[0][2] = s;
	y.values2D[1][0] = 0;
	y.values2D[1][1] = 1;
	y.values2D[1][2] = 0;
	y.values2D[2][0] = -s;
	y.values2D[2][1] = 0;
	y.values2D[2][2] = c;

	return ((*this) * y);
}
//Roatation runt z axeln i "r" grader
inline matrix3D rot_z(float r)
{
	matrix3D z;
	float c = cos(r * PI / 180.0);
	float s = sin(r * PI / 180.0);

	z.values2D[0][0] = c;
	z.values2D[0][1] =-s;
	z.values2D[0][2] = 0;
	z.values2D[1][0] = s;
	z.values2D[1][1] = c;
	z.values2D[1][2] = 0;
	z.values2D[2][0] = 0;
	z.values2D[2][1] = 0;
	z.values2D[2][2] = 1;

	return ((*this) * z);
}
//Roatation runt en vector i "r" grader
inline matrix3D rot_v(vector3D v, float r)
{
	matrix3D temp;

	float c = cos(r * PI / 180.0);
	float s = sin(r * PI / 180.0);
	float x = v.Varray[0];
	float y = v.Varray[1];
	float z = v.Varray[2];

	temp.values2D[0][0] = pow(x, 2) + (1 - pow(x, 2)) * c;
	temp.values2D[0][1] = x * y * (1-c) - x * s;
	temp.values2D[0][2] = x * z * (1-c) + y * s;
	temp.values2D[1][0] = x * y * (1-c) + z * s;
	temp.values2D[1][1] = pow(y, 2) + (1 - pow(y, 2)) * c;
	temp.values2D[1][2] = x * z * (1 - c) - x * s;
	temp.values2D[2][0] = x * z * (1 - c) - y * s;
	temp.values2D[2][1] = y * z * (1 - c) + x * s;
	temp.values2D[2][2] = pow(z, 2) + (1 - pow(z, 2)) * c;

	return temp;
}
//Gångrar en matris med en matirs
inline matrix3D& operator*(const matrix3D& m)
{
	matrix3D temp(*this);
	matrix3D m2;

	if (temp.COLUMN != m.ROW)
	{
		throw;
	}
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			m2.values2D[i][j] = 0;

			for (int k = 0; k < m2.COLUMN; k++)
			{
				m2.values2D[i][j] += temp.values2D[i][k] * m.values2D[k][j];
			}
		}
	}	
	return m2;
}
//Gångrar en matris med en vector
inline const vector3D& operator*(const vector3D& v)
{
	vector3D temp;
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
inline friend std::ostream& operator<<(std::ostream& os, matrix3D& m)
{
	for (int i = 0; i<m.ROW; i++)
	{
		for (int j = 0; j<m.COLUMN; j++)
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
inline matrix3D& operator~()
{
	matrix3D temp;

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			temp.values2D[j][i] = values2D[i][j];
		}
	}

	return temp;
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
inline matrix3D adj(void)
{
	matrix3D temp;

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
inline matrix3D inv()
{
	matrix3D temp;
	matrix3D ajd = this->adj();

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

