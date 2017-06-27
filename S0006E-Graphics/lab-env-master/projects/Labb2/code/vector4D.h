#include <iostream>
#include <cmath>


class vector4D
{
public:


float Varray[4];
int SIZE = 4;

vector4D(void)
{
	Varray[0] = 0;
	Varray[1] = 0;
	Varray[2] = 0;
	Varray[3] = 1;
}

vector4D(float x, float y, float z, float w)
{
	Varray[0] = x;
	Varray[1] = y;
	Varray[2] = z;
	Varray[3] = w;
}

~vector4D(void)
{
}
//Överlagrar brackets för att kunna skriva a[x] = 1;
float& operator[](int n)
{

	return this->Varray[n];

}
//Tillåter addition med en vektor och en vektor
vector4D& operator+(const vector4D& m)
{

	vector4D temp = *this;

	for (int i = 0; i < SIZE; i++)
	{
		temp.Varray[i] += m.Varray[i];
	}


	
	return temp;

}
//Tillåter subtraktion med en vektor och en vektor
vector4D& operator-(const vector4D& m)
{

	vector4D temp = *this;

	for (int i = 0; i < SIZE; i++)
	{
		temp.Varray[i] -= m.Varray[i];
	}


	
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



	return temp;

}
//Skalärprodukten
float dott(vector4D m)
{
	float p = 0;

	for (int i = 0; i<this->length(); i++)
	{
		p += this->Varray[i] * m.Varray[i];
	}

	


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
//Tillåter en att skriva ut en vector med ostream
friend std::ostream& operator<<(std::ostream& os, vector4D m)
{
	for (int i = 0; i<m.SIZE+1; i++)
	{
		os << m.Varray[i] << "\t";


		os << std::endl;
	}

	return os;

}
//Sätter alla värden i vektorn till "v"
void setValues(float v)
{
	for (int i = 0; i<SIZE; i++)
	{

		this->Varray[i] = v;

	}
}
//Ger längden av en vektor
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

