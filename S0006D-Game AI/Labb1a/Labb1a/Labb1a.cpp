// Labb1a.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "Workers.h"
#include <iostream>
#include <windows.h>
#include "EntityManager.h"


int time;
int day = 1;

int _tmain(int argc, _TCHAR* argv[])
{

	///Första agenten startar i goldmine
	Workers* guy = new Workers(1);
	EntityManager::Instance()->RegisterEntity(guy);
	guy->getStateMachine()->setCurrentState(GoMining::Instance());
	guy->setLocation(goldmine);

	///Andra agenten startar i forest
	Workers* guy2 = new Workers(2);
	EntityManager::Instance()->RegisterEntity(guy2);
	guy2->getStateMachine()->setCurrentState(GoLogging::Instance());
	guy2->setLocation(forest);

	///Tredje agenten startar i fishingPond
	Workers* guy3 = new Workers(3);
	EntityManager::Instance()->RegisterEntity(guy3);
	guy3->getStateMachine()->setCurrentState(GoFishing::Instance());
	guy3->setLocation(fishingPond);

	///Fjärde agenten startar i market
	Workers* guy4 = new Workers(4);
	EntityManager::Instance()->RegisterEntity(guy4);
	guy4->getStateMachine()->setCurrentState(WorkAtMarket::Instance());
	guy4->setLocation(market);

	while (1)
	{

		///Håller koll på dag och tid
		if (time <= 24)
		{
			std::cout << time << ":00 day " << day << "\n\n";
			time++;
		}
		else
		{
			time = 0;
			day++;
		}

		std::cout << "Gubbe:" << " Fatigue:" << guy->getFatigue() << " Hunger:" << guy->getHunger() << " Morale:" << guy->getMorale() << " Thrist:" << guy->getThirst() << "\n";
		guy->Update();
		Sleep(1600);
		std::cout << "\n\n";

		std::cout << "Gumma:" << " Fatigue:" << guy2->getFatigue() << " Hunger:" << guy2->getHunger() << " Morale:" << guy2->getMorale() << " Thrist:" << guy2->getThirst() << "\n";
		guy2->Update();
		Sleep(1600);
		std::cout << "\n\n";
 
		std::cout << "Alien:" << " Fatigue:" << guy3->getFatigue() << " Hunger:" << guy3->getHunger() << " Morale:" << guy3->getMorale() << " Thrist:" << guy3->getThirst() << "\n";
		guy3->Update();
		Sleep(1600);
		std::cout << "\n\n";


		std::cout << "Douche:" << " Fatigue:" << guy4->getFatigue() << " Hunger:" << guy4->getHunger() << " Morale:" << guy4->getMorale() << " Thrist:"<<guy4->getThirst() << "\n";
		guy4->Update();
		Sleep(1600);
		std::cout << "\n\n";


	}

	return 0;
}

