#pragma once
#include "Scout.h"
#include <stdlib.h>
#include "Pathfinder.h"
#include "SteeringBehavior.h"
#include "SettingsLoader.h"




Scout::Scout(
			 glm::vec2 velocity, 
			 float mass,
			 float max_force,
			 float max_speed) : MovingEntity(
			 (Pathfinder::Instance()->getStart().getPos()),
			 velocity,
			 max_speed,
			 mass,
			 max_force)
{
	//Statemachine
	m_stateMachine = new StateMachine<Scout>(this);

	//set up the steering behavior class
	steering = new SteeringBehavior<Scout>(this);

	//set the
	setPath(Pathfinder::Instance());
	tempGraph = &getPath()->getGraph();

	mgr = EntityManager::Instance();

	setStartNode(getPath()->getStart());
	setLocationNode(getPath()->getStart());

}

void Scout::Update(float time_elapsed)
{	
	//Sets next pos to current pos so that the entity dosent keep walking after stopping att goal pos
	getNexPos() = Pos();

	//Update entity
	m_stateMachine->Update();

	glm::vec2 SteeringForce;

	//Calculate steeringForce
	SteeringForce = steering->Calculate(1);


	//Calculate acceleration
	glm::vec2 acceleration = SteeringForce / mass;

	//update velocity
	velocity += acceleration * time_elapsed; /*time_elapsed*/;

	//Cap velocity to maxVelocity
	if (velocity.length() > maxSpeed)
	{
		glm::normalize(velocity);

		velocity *= maxSpeed;
	}

	//If the entity is in a marsh tile the speed is reduced.
	if (this->getLocationNode().getType() == marsh)
	{
		velocity /= SettingsLoader::Instance()->getMarshSpeedDivide();
	}

	//update the position
	vPos += velocity * time_elapsed; /*time_elapsed*/;
	
}

void Scout::ChangeLocation(location_type l)
{
	m_Location = l;
}

location_type Scout::getLocation()
{
	return m_Location;
}

bool Scout::HandleMessage(const Telegram& msg)
{
	return m_stateMachine->HandleMessage(msg);
}


