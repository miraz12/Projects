#pragma once
#include "Entity.h"
#include "EntityStates.h"
#include "StateMachine.h"

///Beskriver agenterna
class Workers : public Entity
{
private:

	StateMachine<Workers>* m_stateMachine;

	location_type m_Location;



	int m_gold;
	int m_wood;
	int m_fish;

	int m_currency;

	int m_morale;
	int m_thirst;
	int m_fatigue;
	int m_hunger;



public:
	///Skapar en agent med id: id
	Workers(int id);

	///körs varje loop instans
	void Update();

	///Byter agentens plats
	void ChangeLocation(location_type l);

	//location_type Location();
	location_type getLocation();

	///Dom olika statsen kallar dessa funktioner för att ge agenten olika saker
	void AddToGoldCarried(const int val);
	void AddToWoodCarried(const int val);
	void AddToFishCarried(const int val);
	void Steal();

	///Används för att sätta eller kolla olika värden på agenten

	void BoostMorale(){m_morale = 100;};
	bool Lonely(){ return m_morale <= 60 && m_morale >= 40; }
	bool Depressed(){ return m_morale < 40; }

	bool PocketsFull();
	bool IsTiered(){ return m_fatigue <= 10; }

	void IncreaseFatigue(){ m_fatigue += 5; }
	void DecreaseFatigue(){ m_fatigue -= 1; }
	bool rested(){ return m_fatigue >= 30; };

	bool IsThirsty(){ return m_thirst <= 5; }
	void Drink(){ if (m_thirst < 15){ m_thirst += 5; } }

	bool Hungry(){ return m_hunger <= 5; }
	void EatAtBar();
	bool Full(){ return m_hunger >= 20; };

	void SellGoods(){ m_currency += 20; m_gold = 0; m_wood = 0; m_fish = 0; };
	void BuyStuff();

	~Workers(){ delete m_stateMachine; };

	StateMachine<Workers>* getStateMachine() const { return m_stateMachine; }
	void setStateMachine(StateMachine<Workers>* val) { m_stateMachine = val; }

	virtual bool  HandleMessage(const Telegram& msg);

	///Getters and setter:
	int getFatigue() const { return m_fatigue; }
	void setFatigue(int val) { m_fatigue = val; }

	int getCurrency() const { return m_currency; }

	int getWood() const { return m_wood; }
	void setWood(int val) { m_wood = val; }

	int getFish() const { return m_fish; }
	void setFish(int val) { m_fish = val; }

	int getGold() const { return m_gold; }
	void setGold(int val) { m_gold = val; }

	int getHunger() const { return m_hunger; }
	void setHunger(int val) { m_hunger = val; }

	void setThirst(int val){ m_thirst = val; };
	int getThirst() const { return m_thirst; }

	void setMorale(int val) { m_morale = val; }
	int getMorale(){ return m_morale; }

	void setLocation(location_type val) { m_Location = val; }


};

