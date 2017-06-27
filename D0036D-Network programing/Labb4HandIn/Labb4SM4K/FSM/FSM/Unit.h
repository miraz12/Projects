#pragma once
#include "Object.h"
#include <iostream>

class Unit : public Object
{
public:
	Unit(const glm::vec3 &color);
	~Unit();
	void update(float deltaTime);

};

