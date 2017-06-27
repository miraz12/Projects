#pragma once
#include <iostream>
#include <cmath>

class vector3D
{
public:
float Varray[3];



vector3D(void)
{
	Varray[0] = 0;
	Varray[1] = 0;
	Varray[2] = 0;
}

vector3D(float x, float y, float z)
{
	Varray[0] = x;
	Varray[1] = y;
	Varray[2] = z;
}

~vector3D(void)
{
	
}
//�verlagrar brackets f�r att kunna skriva a[x] = 1;
float& operator[](int n)
{

	return this->Varray[n];

}
//Till�ter addition med en vektor och en vektor
inline vector3D operator+(const vector3D& m)
{

	vector3D temp = *this;

	for (int i = 0; i < 3; i++)
	{
		temp.Varray[i] += m.Varray[i];
	}
	return temp;
}
inline bool operator==(const vector3D& m) const
{
	return this->Varray[0] == m.Varray[0] && this->Varray[1] == m.Varray[1] && this->Varray[2] == m.Varray[2];
}

//Till�ter subtraktion med en vektor och en vektor
inline vector3D operator-(const vector3D& m)
{
	vector3D temp = *this;
	temp.Varray[0] -= m.Varray[0];
	temp.Varray[1] -= m.Varray[1];
	temp.Varray[2] -= m.Varray[2];
	return temp;
}
//Skickar tillbaka produkten av en vektor och en float
inline vector3D operator*(float m)
{
	vector3D temp = *this;
	for (int i = 0; i < 3; i++)
	{
		temp.Varray[i] *= m;
	}
	return temp;
}

inline vector3D operator*(vector3D& other)
{
	vector3D temp = *this;
	for (int i = 0; i < 3; i++)
	{
		temp.Varray[i] *= other[i];
	}
	return temp;
}
inline void operator+=(const vector3D& other)
{
	this->Varray[0] += other.Varray[0];
	this->Varray[1] += other.Varray[1];
	this->Varray[2] += other.Varray[2];
}

//Skal�rprodukten
inline float dot(vector3D m)
{
	float p = 0;
	p = (Varray[0] * m[0]) + (Varray[1] * m[1]) + (Varray[2] * m[2]);
	return p;
}
//Kryssprodukten
inline vector3D cross(vector3D m)
{
	vector3D v;
	v.Varray[0] = (Varray[1] * m[2]) - (Varray[2] * m[1]);
	v.Varray[1] = (Varray[2] * m[0]) - (Varray[0] * m[2]);
	v.Varray[2] = (Varray[0] * m[1]) - (Varray[1] * m[0]);
	return v;
}
//Ger l�ngden av en vektor
inline float length(void)
{
	float l = 0;
	for (int i = 0; i < 3; i++)
	{
		l += powf(this->Varray[i], 2.0f);
	}
	return sqrtf(l);
}
//Ger l�ngden av en vektor
inline float sqrlength(void)
{
	float l = 0;
	for (int i = 0; i < 3; i++)
	{
		l += powf(this->Varray[i], 2.0f);
	}
	return l;
}
//Normaliserar vektorn
inline void normalize(void)
{
	float l = length();
	if (l != 0){
		Varray[0] = Varray[0] / l;
		Varray[1] = Varray[1] / l;
		Varray[2] = Varray[2] / l;
	}
}
inline vector3D normalizeRe(void)
{
	vector3D temp;
	float l = length();
	if (l != 0){
		temp[0] = Varray[0] / l;
		temp[1] = Varray[1] / l;
		temp[2] = Varray[2] / l;
	}
	return temp;
}


inline vector3D normalize(vector3D v)
{

	float l = this->length();


	for (int i = 0; i < 3; i++)
	{
		v[i] = (v[i] / l);
	}

	return v;
}
//S�tter alla v�rden i vektorn till "v"
inline void setValues(float v)
{
	for (int i = 0; i < 3; i++)
	{

		this->Varray[i] = v;

	}
}
inline void setValues(float x, float y, float z)
{
    this->Varray[0] = x;
    this->Varray[1] = y;
    this->Varray[2] = z;
}
//Till�ter en att skriva ut en vector med ostream
inline friend std::ostream& operator<<(std::ostream& os, vector3D m)
{
	for (int i = 0; i < 3; i++)
	{
		os << m.Varray[i] << "\t";


		os << std::endl;
	}

	return os;

}

inline friend vector3D operator*(float& f, vector3D m)
{
	vector3D temp = m;
	for (int i = 0; i < 3; i++)
	{
		temp.Varray[i] *= f;
	}
	return temp;
}

inline void addScaledVector(vector3D v, double scale)
{
	this->Varray[0] += v[0] * scale;
	this->Varray[1] += v[1] * scale;
	this->Varray[2] += v[2] * scale;
}

inline float x()
{
	return this->Varray[0];
}
inline float y()
{
	return this->Varray[1];
}
inline float z()
{
	return this->Varray[2];
}

};

