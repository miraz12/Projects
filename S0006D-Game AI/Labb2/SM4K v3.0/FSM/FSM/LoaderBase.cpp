#include "LoaderBase.h"
#include <assert.h>


LoaderBase::LoaderBase(char* filename)
{
	file.open(filename);

	assert(file && "Load ERROR");
}


LoaderBase::~LoaderBase()
{
}

float LoaderBase::GetNextParameterFloat()
{
	//casta om värdet till en float
	return (float)atof(GetNextParameter().c_str());

}

std::string LoaderBase::GetNextParameter()
{
	std::string line;

	std::getline(file, line);

	if (line.length() == 0)
	{
		return GetNextParameter();
	}

	GetParameterValueAsString(line);

	return line;
}
void LoaderBase::GetParameterValueAsString(std::string& line)
{
	std::string::size_type startIndex;
	std::string::size_type endIndex;

	const std::string delims(" \=,");

	startIndex = line.find_first_not_of(delims);

	if (startIndex != std::string::npos)
	{
		endIndex = line.find_first_of(delims, startIndex);

		if (endIndex == std::string::npos)
		{
			endIndex = line.length();
		}
	}

	startIndex = line.find_first_not_of(delims, endIndex);

	if (startIndex != std::string::npos)
	{
		endIndex = line.find_first_of(delims, startIndex);

		if (endIndex == std::string::npos)
		{
			endIndex = line.length();
		}
	}

	line = line.substr(startIndex, endIndex);
}
