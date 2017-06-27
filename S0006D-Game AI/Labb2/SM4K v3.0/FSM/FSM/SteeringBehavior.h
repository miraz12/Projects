#pragma once
#include "Scout.h"
#include "State.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

///Hold the different SteeringBehaviors
template <class entity_type>
class SteeringBehavior
{
private:

	//a pointer to the owner of this instance
	entity_type* movEntity;

	//the steering force created by the combined effect of all
	//the selected behaviors
	glm::vec2 steeringForce;

	//Used to choose wich kind of behevior to use. Not really nessercy in this lab but can be used when implementig more beheviors
	int behaviorType;

public:

	SteeringBehavior(entity_type* agent) : movEntity(agent)
	{
	};

	~SteeringBehavior()
	{
		delete movEntity;
	};
	//Calculate steering force with currPos and targetPos. if they are the same return 0 velocity
	glm::vec2 Seek(glm::vec2 TargetPos)
	{
		if ((TargetPos - movEntity->Pos()) != glm::vec2(0.0f, 0.0f))
		{
			glm::vec2 DesiredVelocity = glm::normalize(TargetPos - movEntity->Pos()) * movEntity->getMaxSpeed();
			return (DesiredVelocity - movEntity->getVelocity());
		}
		else
		{
			return glm::vec2(0.0f, 0.0f);
		}		
	};

	
	glm::vec2 Calculate(int behavior)
	{
		behaviorType = behavior;

		steeringForce = glm::vec2(0.0f,0.0f);

		steeringForce = CalculateWeightedSum(); 

		return steeringForce;
	};

	glm::vec2 SteeringBehavior::CalculateWeightedSum()
	{

		if (behaviorType = 1)
		{
			return steeringForce += Seek(movEntity->getNexPos()) * movEntity->getMass();
		}
		
		else if (behaviorType = 2)
		{

		}
		
	}

	Scout* getVehicle() const { return movEntity; }
	void setVehicle(Scout* val) { movEntity = val; }
};





