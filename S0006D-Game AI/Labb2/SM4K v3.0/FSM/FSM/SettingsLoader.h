#pragma once
#include "LoaderBase.h"
///Sets variables to values from file
class SettingsLoader : public LoaderBase
{
private:
	//Ent max vel
	float entMaxVelocity;

	//Uppgrade times
	float upgradeScoutTime;
	float upgradeBuilderTime;
	float upgradeKilnerTime;

	//Num of dif ents
	float numberOfScouts;
	float numberOfBuilders;
	float numberOfKilners;

	//Working times
	float chopTreeTime;
	float buildKilnTime;
	float burnTreeTime;
	
	float marshSpeedDivide;



public:


	SettingsLoader();
	~SettingsLoader();
	static SettingsLoader* Instance();

	//Geters:
	float getentMaxVelocity() const { return entMaxVelocity; }

	float getUpgradeScoutTime() const { return upgradeScoutTime; }
	float getUpgradeBuilderTime() const { return upgradeBuilderTime; }
	float getUpgradeKilnerTime() const { return upgradeKilnerTime; }

	float getNumberOfScouts() const { return numberOfScouts; }
	float getNumberOfBuilders() const { return numberOfBuilders; }
	float getNumberOfKilners() const { return numberOfKilners; }

	float getChopTreeTime() const { return chopTreeTime; }
	float getBuildKilnTime() const { return buildKilnTime; }
	float getBurnTreeTime() const { return burnTreeTime; }

	float getMarshSpeedDivide() const { return marshSpeedDivide; }

};

