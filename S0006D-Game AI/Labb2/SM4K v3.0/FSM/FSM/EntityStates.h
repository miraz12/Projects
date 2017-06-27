#pragma once
#include "State.h"

class Scout;
class Worker;
class Builder;
class Kilner;


//////////////////////////////////////////////////////////////
class ScoutMap : public State<Scout>
{

private:
	ScoutMap()
	{
	};

	bool occupied = false;


public:

	virtual void StateEnter(Scout* worker);

	virtual void Execute(Scout* worker);

	virtual void StateExit(Scout* worker);

	//Singleton
	static ScoutMap* Instance();

	bool OnMessage(Scout* worker, const Telegram& msg);

};

//////////////////////////////////////////////////////////////
class SearchTree : public State<Worker>
{

private:
	SearchTree()
	{
	};

	bool occupied = false;


public:

	virtual void StateEnter(Worker* worker);

	virtual void Execute(Worker* worker);

	virtual void StateExit(Worker* worker);

	//Singleton
	static SearchTree* Instance();

	bool OnMessage(Worker* worker, const Telegram& msg);

};

//////////////////////////////////////////////////////////////
class IdleWorker : public State<Worker>
{

private:
	IdleWorker()
	{
	};

	bool occupied = false;


public:

	virtual void StateEnter(Worker* worker);

	virtual void Execute(Worker* worker);

	virtual void StateExit(Worker* worker);

	//Singleton
	static IdleWorker* Instance();

	bool OnMessage(Worker* worker, const Telegram& msg);

};

//////////////////////////////////////////////////////////////
class ChopTree : public State<Worker>
{

private:
	ChopTree()
	{
	};

	bool occupied = false;


public:

	virtual void StateEnter(Worker* worker);

	virtual void Execute(Worker* worker);

	virtual void StateExit(Worker* worker);

	//Singleton
	static ChopTree* Instance();

	bool OnMessage(Worker* worker, const Telegram& msg);

};
//////////////////////////////////////////////////////////////
class ChopChop : public State<Worker>
{

private:
	ChopChop()
	{
	};

	bool occupied = false;


public:

	virtual void StateEnter(Worker* worker);

	virtual void Execute(Worker* worker);

	virtual void StateExit(Worker* worker);

	//Singleton
	static ChopChop* Instance();

	bool OnMessage(Worker* worker, const Telegram& msg);

};

//////////////////////////////////////////////////////////////
class UppgradeToScout : public State<Worker>
{

private:
	UppgradeToScout()
	{
	};

	bool occupied = false;


public:

	virtual void StateEnter(Worker* worker);

	virtual void Execute(Worker* worker);

	virtual void StateExit(Worker* worker);

	//Singleton
	static UppgradeToScout* Instance();

	bool OnMessage(Worker* worker, const Telegram& msg);

};
//////////////////////////////////////////////////////////////
class UppgradeToBuilder : public State<Worker>
{

private:
	UppgradeToBuilder()
	{
	};

	bool occupied = false;


public:

	virtual void StateEnter(Worker* worker);

	virtual void Execute(Worker* worker);

	virtual void StateExit(Worker* worker);

	//Singleton
	static UppgradeToBuilder* Instance();

	bool OnMessage(Worker* worker, const Telegram& msg);

};
//////////////////////////////////////////////////////////////
class UppgradeToKilner : public State<Worker>
{

private:
	UppgradeToKilner()
	{
	};

	bool occupied = false;


public:

	virtual void StateEnter(Worker* worker);

	virtual void Execute(Worker* worker);

	virtual void StateExit(Worker* worker);

	//Singleton
	static UppgradeToKilner* Instance();

	bool OnMessage(Worker* worker, const Telegram& msg);

};
//////////////////////////////////////////////////////////////
class WorkerMove : public State<Worker>
{

private:
	WorkerMove()
	{
	};

	bool occupied = false;


public:

	virtual void StateEnter(Worker* worker);

	virtual void Execute(Worker* worker);

	virtual void StateExit(Worker* worker);

	//Singleton
	static WorkerMove* Instance();

	bool OnMessage(Worker* worker, const Telegram& msg);

};

//////////////////////////////////////////////////////////////
class TransportToBase : public State<Worker>
{

private:
	TransportToBase()
	{
	};

	bool occupied = false;


public:

	virtual void StateEnter(Worker* worker);

	virtual void Execute(Worker* worker);

	virtual void StateExit(Worker* worker);

	//Singleton
	static TransportToBase* Instance();

	bool OnMessage(Worker* worker, const Telegram& msg);

};
//////////////////////////////////////////////////////////////
class BuildCharcoalKiln : public State<Builder>
{

private:
	BuildCharcoalKiln(){};


public:

	virtual void StateEnter(Builder* worker);

	virtual void Execute(Builder* worker);

	virtual void StateExit(Builder* worker);

	//Singleton
	static BuildCharcoalKiln* Instance();

	bool OnMessage(Builder* worker, const Telegram& msg);

};
//////////////////////////////////////////////////////////////////
class MoveToBuildSite : public State<Builder>
{

private:
	MoveToBuildSite(){};


public:

	virtual void StateEnter(Builder* worker);

	virtual void Execute(Builder* worker);

	virtual void StateExit(Builder* worker);

	//Singleton
	static MoveToBuildSite* Instance();

	bool OnMessage(Builder* worker, const Telegram& msg);

};
//////////////////////////////////////////////////////////////////
class WorkAtKiln : public State<Kilner>
{

private:
	WorkAtKiln(){};


public:

	virtual void StateEnter(Kilner* worker);

	virtual void Execute(Kilner* worker);

	virtual void StateExit(Kilner* worker);

	//Singleton
	static WorkAtKiln* Instance();

	bool OnMessage(Kilner* worker, const Telegram& msg);

};
//////////////////////////////////////////////////////////////////
class MoveToKiln : public State<Kilner>
{

private:
	MoveToKiln(){};


public:

	virtual void StateEnter(Kilner* worker);

	virtual void Execute(Kilner* worker);

	virtual void StateExit(Kilner* worker);

	//Singleton
	static MoveToKiln* Instance();

	bool OnMessage(Kilner* worker, const Telegram& msg);

};