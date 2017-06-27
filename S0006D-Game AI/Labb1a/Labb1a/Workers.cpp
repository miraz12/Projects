#pragma once
#include "Workers.h"
#include <stdlib.h>


Workers::Workers(int id) :m_gold(0),
				  m_currency(5), 
				  m_Location(home),
				  m_fatigue(rand() % 30 + 1),
				  m_thirst(rand() % 15 + 1),
				  m_hunger(rand() % 20 + 10),
				  m_morale(rand() % 100 + 1),
				  Entity(id)
{

	m_stateMachine = new StateMachine<Workers>(this);

	//m_stateMachine->ChangeStateNotWorking(GoMining::Instance());

}

void Workers::Update()
{
	
	m_thirst--;
	m_hunger--;
	m_morale -= 10;
	m_stateMachine->Update();

	if (m_thirst <= 0 || m_hunger <= 0)
	{
		m_stateMachine->ChangeState(EntityDead::Instance());
	}
	
}

void Workers::ChangeLocation(location_type l)
{
	m_Location = l;
}

location_type Workers::getLocation()
{
	return m_Location;
}

void Workers::AddToGoldCarried(const int val)
{
	this->m_gold += val;
}
void Workers::AddToWoodCarried(const int val)
{
	this->m_wood += val;
}
void Workers::AddToFishCarried(const int val)
{
	this->m_fish += val;
}


void Workers::Steal()
{
	int ran = rand() % 100 + 1;

	if (ran < 30)
	{
		this->m_currency++;
	}

}

bool Workers::PocketsFull()
{
	int carryCap = 15;
	return m_gold >= carryCap || m_wood >= carryCap || m_fish >= carryCap;
}

void Workers::EatAtBar()
{
	m_currency--;
	m_hunger += 5;
	m_thirst += 2;
}

bool Workers::HandleMessage(const Telegram& msg)
{
	return m_stateMachine->HandleMessage(msg);
}


