#pragma once
#include "Entity.h"
#include <iostream>


Entity::Entity(int id)
{
	if (id >= m_nextValidID)
	{
		m_ID = id;

		m_nextValidID = m_ID + 1;
	}
	else
	{
		std::cout << "ERROR: Invalid ID";
	}

}


Entity::~Entity()
{
}





