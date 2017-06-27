#pragma once
#include "Telegram.h"
#include "Locations.h"


class Entity
{
public:
	Entity(int id);
	~Entity();

	///Körs varje loop
	virtual void Update() = 0;

	///Unikt id för varje agent
	int getID(){ return m_ID; };

	///Meddelande hantering
	virtual bool HandleMessage(const Telegram& msg) = 0;

	///ger agentens postion
	virtual location_type getLocation() = 0;


private:
	int m_ID;
	int m_nextValidID;

	void SetID(int val);

};

