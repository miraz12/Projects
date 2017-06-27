#pragma once
#include "Entity.h"

///Moving agents base
class MovingEntity : public Entity
{
protected:

	//Current velocity
	glm::vec2    velocity;

	//entity weight
	float       mass;

	//max entity speed
	float       maxSpeed;

	//maximum force
	float        maxForce;

public:

	MovingEntity(glm::vec2 position,
		glm::vec2 velocity,
		double    max_speed,
		double    mass,
		double    max_force) :Entity(position),
		velocity(velocity),
		mass(mass),
		maxSpeed(max_speed),
		maxForce(max_force)
	{}

	virtual ~MovingEntity(){};


	glm::vec2 getVelocity() const { return velocity; }
	void setVelocity(glm::vec2 val) { velocity = val; }

	float getMass() const { return mass; }
	void setMass(float val) { mass = val; }

	float getMaxSpeed() const { return maxSpeed; }
	void setMaxSpeed(float val) { maxSpeed = val; }

	float getMaxForce() const { return maxForce; }
	void setMaxForce(float val) { maxForce = val; }
};

