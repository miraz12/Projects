#pragma once
#include "Entity.h"
#include "Entity.h"
#include "EntityStates.h"
#include "StateMachine.h"
#include "Pathfinder.h"
#include "NavNode.h"
#include "MovingEntity.h"
#include "EntityManager.h"

///Builder entity
template<class entity_type>
class SteeringBehavior;

class Builder : public MovingEntity
{
private:
	StateMachine<Builder>* m_stateMachine;

	//the steering behavior class
	SteeringBehavior<Builder>* steering;;

	location_type m_Location;

	std::list<int>* visIndex;

	EntityManager* mgr;

	bool working;

	float startTime;


public:
	Builder(
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


	~Builder()
	{ 

	};

	StateMachine<Builder>* getStateMachine() const { return m_stateMachine; }
	void setStateMachine(StateMachine<Builder>* val) { m_stateMachine = val; }

	virtual bool  HandleMessage(const Telegram& msg);


	float& getStartTime() { return startTime; }

};

