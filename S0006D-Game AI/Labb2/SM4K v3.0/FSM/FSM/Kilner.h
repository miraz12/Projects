#pragma once
#include "Entity.h"
#include "Entity.h"
#include "EntityStates.h"
#include "StateMachine.h"
#include "Pathfinder.h"
#include "NavNode.h"
#include "MovingEntity.h"
#include "EntityManager.h"

template<class entity_type>
class SteeringBehavior;

///Kilner entity
class Kilner : public MovingEntity
{
private:
	StateMachine<Kilner>* m_stateMachine;

	//the steering behavior class
	SteeringBehavior<Kilner>* steering;;

	EntityManager* mgr;

	location_type m_Location;

	std::list<int>* visIndex;

	bool working;


	float startTime;

public:

	///Skapar en agent med id: id
	//Kilner(int id);

	Kilner(
		glm::vec2 velocity,
		float    mass,
		float    max_force,
		float    max_speed);

	///körs varje loop instans
	void Update(float time_elapsed);

	///Byter agentens plats
	void ChangeLocation(location_type l);

	//location_type Location();
	location_type getLocation();


	~Kilner()
	{

	};

	StateMachine<Kilner>* getStateMachine() const { return m_stateMachine; }
	void setStateMachine(StateMachine<Kilner>* val) { m_stateMachine = val; }

	virtual bool  HandleMessage(const Telegram& msg);
	float& getStartTime() { return startTime; }



};

