#pragma once
#include "State.h"

class Workers;


//////////////////////////////////////////////////////////////
class GoMining : public State<Workers>
{

private:
	GoMining(){};

	bool occupied = false;


public:

	virtual void StateEnter(Workers* worker);

	virtual void Execute(Workers* worker);

	virtual void StateExit(Workers* worker);

	//Singleton
	static GoMining* Instance();

	bool OnMessage(Workers* worker, const Telegram& msg);

};

//////////////////////////////////////////////////////////

class GoToMarket : public State<Workers>
{
private:
	GoToMarket(){};

public:
	//singleton
	static GoToMarket* Instance();


	virtual void StateEnter(Workers* worker);

	virtual void Execute(Workers* worker);

	virtual void StateExit(Workers* worker);

	bool OnMessage(Workers* worker, const Telegram& msg);
};

//////////////////////////////////////////////////////////

class GoToWell : public State<Workers>
{
private:
	GoToWell(){};


public:

	static GoToWell* Instance();

	virtual void StateEnter(Workers* worker);

	virtual void Execute(Workers* worker);

	virtual void StateExit(Workers* worker);

	bool OnMessage(Workers* worker, const Telegram& msg);

};

/////////////////////////////////////////////////////////

class GoLogging : public State<Workers>
{
private:
	GoLogging(){};

	bool occupied = false;

public:

	static GoLogging* Instance();

	virtual void StateEnter(Workers* worker);

	virtual void Execute(Workers* worker);

	virtual void StateExit(Workers* worker);

	bool OnMessage(Workers* worker, const Telegram& msg);

};

//////////////////////////////////////////////////////////////////

class GoFishing : public State<Workers>
{
private:
	GoFishing(){};

	bool occupied = false;

public:

	static GoFishing* Instance();

	virtual void StateEnter(Workers* worker);

	virtual void Execute(Workers* worker);

	virtual void StateExit(Workers* worker);

	bool OnMessage(Workers* worker, const Telegram& msg);

};

/////////////////////////////////////////////////////////////////

class GoHome : public State<Workers>
{
private:
	GoHome(){};


public:

	static GoHome* Instance();

	virtual void StateEnter(Workers* worker);

	virtual void Execute(Workers* worker);

	virtual void StateExit(Workers* worker);

	bool OnMessage(Workers* worker, const Telegram& msg);

};

/////////////////////////////////////////////////////////////////

class WorkAtMarket : public State<Workers>
{
private:
	WorkAtMarket(){};

public:

	static WorkAtMarket* Instance();

	virtual void StateEnter(Workers* worker);

	virtual void Execute(Workers* worker);

	virtual void StateExit(Workers* worker);

	bool OnMessage(Workers* worker, const Telegram& msg);
};

/////////////////////////////////////////////////////////////////////


class EatAtBar : public State<Workers>
{
private:
	EatAtBar(){};

public:

	static EatAtBar* Instance();

	virtual void StateEnter(Workers* worker);

	virtual void Execute(Workers* worker);

	virtual void StateExit(Workers* worker);

	bool OnMessage(Workers* worker, const Telegram& msg);
};

///////////////////////////////////////////////////////////////////


class EntityDead : public State<Workers>
{
private:
	EntityDead(){};

public:

	static EntityDead* Instance();

	virtual void StateEnter(Workers* worker);

	virtual void Execute(Workers* worker);

	virtual void StateExit(Workers* worker);

	bool OnMessage(Workers* worker, const Telegram& msg);
};