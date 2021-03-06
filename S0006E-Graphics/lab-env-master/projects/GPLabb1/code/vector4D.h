#include "stdafx.h"

#include <iostream>
#include <cmath>


class vector4D
{
public:


float* Varray;
int SIZE;

vector4D(void)
{
	this->Varray = new float[3];
	this->Varray[3] = 1;
	SIZE = 4 - 1;
}

~vector4D(void)
{
}
//�verlagrar brackets f�r att kunna skriva a[x] = 1;
float& operator[](int n)
{
	return this->Varray[n];
}
//Till�ter addition med en vektor och en vektor
vector4D& operator+(const vector4D& m)
{

	vector4D temp = *this;

	for (int i = 0; i < SIZE; i++)
	{
		temp.Varray[i] += m.Varray[i];
	}


	std::cout << temp;
	return temp;

}
//Till�ter subtraktion med en vektor och en vektor
vector4D& operator-(const vector4D& m)
{

	vector4D temp = *this;

	for (int i = 0; i < SIZE; i++)
	{
		temp.Varray[i] -= m.Varray[i];
	}


	std::cout << temp;
	return temp;

}
//Skickar tillbaka produkten av en vektor och en float
vector4D& operator*(float m)
{

	vector4D temp = *this;

	for (int i = 0; i < SIZE; i++)
	{
		temp.Varray[i] *= m;
	}


	std::cout << temp;
	return temp;

}
//Skal�rprodukten
float dott(vector4D m)
{
	float p = 0;

	for (int i = 0; i<this->length(); i++)
	{
		p += this->Varray[i] * m.Varray[i];
	}

	std::cout << p << "\n";

	return p;

}
//Kryssprodukten
vector4D cross(vector4D m)
{
	vector4D v;

	v.Varray[0] = (this->Varray[1] * m.Varray[2]) - (this->Varray[2] * m.Varray[1]);
	v.Varray[1] = (this->Varray[2] * m.Varray[0]) - (this->Varray[0] * m.Varray[2]);
	v.Varray[2] = (this->Varray[0] * m.Varray[1]) - (this->Varray[1] * m.Varray[0]);

	return v;

}
//Till�ter en att skriva ut en vector med ostream
friend std::ostream& operator<<(std::ostream& os, vector4D m)
{
	for (int i = 0; i<m.SIZE+1; i++)
	{
		os << m.Varray[i] << "\t";


		os << std::endl;
	}

	return os;

}
//S�tter alla v�rden i vektorn till "v"
void setValues(float v)
{
	for (int i = 0; i<SIZE; i++)
	{

		this->Varray[i] = v;

	}
}
//Ger l�ngden av en vektor
float length(void)
{
	float l = 0;

	for (int i = 0; i < SIZE; i++)
	{
		l += pow(this->Varray[i], 2);
	}
	return l;
}
//Normaliserar vektorn
void normalize(void)
{

	float l = this->length();


	for (int i = 0; i<SIZE; i++)
	{
		this->Varray[i] = (this->Varray[i] / l);
	}
}

};

