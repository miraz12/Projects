#pragma once
#include "Entity.h"
#include "EntityStates.h"
#include "StateMachine.h"
#include "Pathfinder.h"
#include "NavNode.h"
#include "MovingEntity.h"
#include "EntityManager.h"

template<class entity_type>
class SteeringBehavior;

///Beskriver agenterna
class Scout : public MovingEntity
{
private:


	StateMachine<Scout>* m_stateMachine;

	//the steering behavior class
	SteeringBehavior<Scout>* steering;;

	location_type m_Location;

	EntityManager* mgr;






public:
// 	///Skapar en agent med id: id
// 	Scout(int id);

	Scout(
		glm::vec2 velocity,
		float    mass,
		float    max_force,
		float    max_speed
		);

	///körs varje loop instans
	void Update(float time_elapsed);

	///Byter agentens plats
	void ChangeLocation(location_type l);

	//location_type Location();
	location_type getLocation();

	~Scout()
	{ 
	};

	StateMachine<Scout>* getStateMachine() const { return m_stateMachine; }
	void setStateMachine(StateMachine<Scout>* val) { m_stateMachine = val; }
	SteeringBehavior<Scout>* getSteering() const { return steering; }



	virtual bool  HandleMessage(const Telegram& msg);

	void setLocation(location_type val) { m_Location = val; }

	EntityManager* getMgr() const { return mgr; }
};

