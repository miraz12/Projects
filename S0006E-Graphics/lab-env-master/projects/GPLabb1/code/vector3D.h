#include "stdafx.h"
#include <iostream>
#include <cmath>

class vector3D
{
public:

	float* Varray;
	int SIZE;

vector3D(void)
{
	this->Varray = new float[3];
	SIZE = 3;
}

vector3D::~vector3D(void)
{
}
//�verlagrar brackets f�r att kunna skriva a[x] = 1;
float& operator[](int n)
{

	return this->Varray[n];

}
//Till�ter addition med en vektor och en vektor
inline vector3D& operator+(const vector3D& m)
{

	vector3D temp = *this;

	for (int i = 0; i < SIZE; i++)
	{
		temp.Varray[i] += m.Varray[i];
	}


	std::cout << temp;
	return temp;

}
//Till�ter subtraktion med en vektor och en vektor
inline vector3D& operator-(const vector3D& m)
{

	vector3D temp = *this;

	for (int i = 0; i < SIZE; i++)
	{
		temp.Varray[i] -= m.Varray[i];
	}


	std::cout << temp;
	return temp;

}
//Skickar tillbaka produkten av en vektor och en float
inline vector3D& operator*(float m)
{

	vector3D temp = *this;

	for (int i = 0; i < SIZE; i++)
	{
		temp.Varray[i] *= m;
	}


	std::cout << temp;
	return temp;

}
//Skal�rprodukten
inline float dott(vector3D m)
{
	float p = 0;

	for (int i = 0; i < this->length(); i++)
	{
		p += this->Varray[i] * m.Varray[i];
	}

	std::cout << p << "\n";

	return p;

}
//Kryssprodukten
inline vector3D cross(vector3D m)
{
	vector3D v;

	v.Varray[0] = (this->Varray[1] * m.Varray[2]) - (this->Varray[2] * m.Varray[1]);
	v.Varray[1] = (this->Varray[2] * m.Varray[0]) - (this->Varray[0] * m.Varray[2]);
	v.Varray[2] = (this->Varray[0] * m.Varray[1]) - (this->Varray[1] * m.Varray[0]);

	return v;

}
//Ger l�ngden av en vektor
inline float length(void)
{
	float l = 0;

	for (int i = 0; i < SIZE; i++)
	{
		l += pow(this->Varray[i], 2);
	}
	return sqrt(l);
}
//Normaliserar vektorn
inline void normalize(void)
{

	float l = this->length();


	for (int i = 0; i < SIZE; i++)
	{
		this->Varray[i] = (this->Varray[i] / l);
	}
}
//S�tter alla v�rden i vektorn till "v"
inline void setValues(float v)
{
	for (int i = 0; i < SIZE; i++)
	{

		this->Varray[i] = v;

	}
}
//Till�ter en att skriva ut en vector med ostream
inline friend std::ostream& operator<<(std::ostream& os, vector3D m)
{
	for (int i = 0; i < m.SIZE; i++)
	{
		os << m.Varray[i] << "\t";


		os << std::endl;
	}

	return os;

}


};

