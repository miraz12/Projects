#pragma once
#include "Entity.h"
#include "EntityStates.h"
#include "StateMachine.h"
#include "Pathfinder.h"
#include "NavNode.h"
#include "MovingEntity.h"
#include "SteeringBehavior.h"

template<class entity_type>
class SteeringBehavior;

///worker entity
class Worker : public MovingEntity
{
private:

	StateMachine<Worker>* m_stateMachine;

	//the steering behavior class
	SteeringBehavior<Worker>* steering;

	location_type m_Location;

	int wood;

	bool handsFull;

	std::list<int>* visIndex;

	float startTime;


public:
	///Skapar en agent med id: id
	//Worker(int id);

	Worker(
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


	~Worker(){ delete m_stateMachine; };

	StateMachine<Worker>* getStateMachine() const { return m_stateMachine; }
	void setStateMachine(StateMachine<Worker>* val) { m_stateMachine = val; }

	virtual bool  HandleMessage(const Telegram& msg);

	void setLocation(location_type val) { m_Location = val; }
	int getWood() const { return wood; }
	void setWood(int val) { wood = val; }
	bool getHandsFull() const { return handsFull; }
	void setHandsFull(bool val) { handsFull = val; }
	std::list<int>* getVisIndex() const { return visIndex; }

	float& getStartTime() { return startTime; }

};
