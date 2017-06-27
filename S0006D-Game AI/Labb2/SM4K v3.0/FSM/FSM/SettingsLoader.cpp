#include "SettingsLoader.h"


SettingsLoader::SettingsLoader() : LoaderBase("settings.ini")
{
	entMaxVelocity = GetNextParameterFloat();

	upgradeScoutTime = GetNextParameterFloat();
	upgradeBuilderTime = GetNextParameterFloat();
	upgradeKilnerTime = GetNextParameterFloat();

	numberOfScouts = GetNextParameterFloat();
	numberOfBuilders = GetNextParameterFloat();
	numberOfKilners = GetNextParameterFloat();

	chopTreeTime = GetNextParameterFloat();
	buildKilnTime = GetNextParameterFloat();
	burnTreeTime = GetNextParameterFloat();

	marshSpeedDivide = GetNextParameterFloat();
}


SettingsLoader::~SettingsLoader()
{

}

SettingsLoader* SettingsLoader::Instance()
{
	static SettingsLoader instance;

	return &instance;
}
