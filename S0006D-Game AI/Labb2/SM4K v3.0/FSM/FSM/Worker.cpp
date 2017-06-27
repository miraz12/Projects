#pragma once
#include "Worker.h"
#include <stdlib.h>
#include "EntityManager.h"
#include "SteeringBehavior.h"
#include "SettingsLoader.h"


Worker::Worker(
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

	m_stateMachine = new StateMachine<Worker>(this);
	//set up the steering behavior class
	steering = new SteeringBehavior<Worker>(this);

	tempGraph = &EntityManager::Instance()->getVisitedGraph();
	visIndex = &EntityManager::Instance()->getVisitedNodes();

	setStartNode(tempGraph->getNode(Pathfinder::Instance()->getStart().getIndex()));
	setLocationNode(tempGraph->getNode(Pathfinder::Instance()->getStart().getIndex()));



}

void Worker::Update(float time_elapsed)
{

	getNexPos() = Pos();
	m_stateMachine->Update();


	

	glm::vec2 OldPos = Pos();

	glm::vec2 SteeringForce;

	SteeringForce = steering->Calculate(1);


	//Acceleration = Force/Mass
	glm::vec2 acceleration = SteeringForce / mass;

	//update velocity
	velocity += acceleration * time_elapsed; /*time_elapsed*/;

	//make sure vehicle does not exceed maximum velocity 	vVelocity.Truncate(dMaxSpeed);
	if (velocity.length() > maxSpeed)
	{
		glm::normalize(velocity);

		velocity *= maxSpeed;
	}


	if (this->getLocationNode().getType() == marsh)
	{
		velocity /= SettingsLoader::Instance()->getMarshSpeedDivide();
	}

	//update the position
	vPos += velocity * time_elapsed; /*time_elapsed*/;

}

void Worker::ChangeLocation(location_type l)
{
	m_Location = l;
}

location_type Worker::getLocation()
{
	return m_Location;
}

bool Worker::HandleMessage(const Telegram& msg)
{
	return m_stateMachine->HandleMessage(msg);
}


