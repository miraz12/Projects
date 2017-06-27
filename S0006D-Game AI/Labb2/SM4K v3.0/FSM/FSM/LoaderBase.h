#pragma once
#include <fstream>
#include <string>
///Loads fila and reads in lines
class LoaderBase
{
private:
	std::ifstream file;

public:
	LoaderBase(char* file);
	~LoaderBase();
	float GetNextParameterFloat();
	std::string GetNextParameter();
	void GetParameterValueAsString(std::string& line);
};

