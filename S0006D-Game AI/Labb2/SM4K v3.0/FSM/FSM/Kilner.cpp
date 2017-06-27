#include "Kilner.h"
#include "EntityManager.h"
#include "SteeringBehavior.h"
#include "SettingsLoader.h"

Kilner::Kilner(
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

	m_stateMachine = new StateMachine<Kilner>(this);

	//set up the steering behavior class
	steering = new SteeringBehavior<Kilner>(this);

	mgr = EntityManager::Instance();
	
	tempGraph = &mgr->getVisitedGraph();
	visIndex = &mgr->getVisitedNodes();

	setStartNode(tempGraph->getNode(Pathfinder::Instance()->getStart().getIndex()));
	setLocationNode(tempGraph->getNode(Pathfinder::Instance()->getStart().getIndex()));

}

void Kilner::Update(float time_elapsed)
{
	getNexPos() = Pos();

	m_stateMachine->Update();

	glm::vec2 OldPos = Pos();

	glm::vec2 SteeringForce;

	SteeringForce = steering->Calculate(2);


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

void Kilner::ChangeLocation(location_type l)
{
	m_Location = l;
}

location_type Kilner::getLocation()
{
	return m_Location;
}

bool Kilner::HandleMessage(const Telegram& msg)
{
	return m_stateMachine->HandleMessage(msg);
}

