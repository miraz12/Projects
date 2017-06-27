#pragma once
#include "Entity.h"
#include <iostream>


Entity::Entity(int id)
{
	if (id >= m_nextValidID)
	{
		m_ID = id;

		m_nextValidID = m_ID + 1;



// 		path = Pathfinder::Instance();
// 		tempGraph = &path->getGraph();

		//locationNode = path->getStart();
		//startNode = EntityManager::Instance()->getGrapRef()->getNode(0);
		/*startNode = EntityManager::Instance()->getGrapRef()->getNode(0);*/

		
	}
	else
	{
		std::cout << "ERROR: Invalid ID";
	}

}


Entity::~Entity()
{
	delete tempGraph;
	delete sprite;
	delete path;

}





