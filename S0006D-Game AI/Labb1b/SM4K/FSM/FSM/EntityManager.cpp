#include "stdafx.h"
#include "EntityManager.h"


EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
}

EntityManager* EntityManager::Instance()
{
	static EntityManager instance;

	return &instance;
}


void EntityManager::RegisterEntity(Entity* newEnt)
{
	m_EntityMap.insert(std::make_pair(newEnt->getID(), newEnt));
}

Entity* EntityManager::GetEntity(int id)
{
	EntityMap::const_iterator ent = m_EntityMap.find(id);

	return ent->second;
}

void EntityManager::RemoveEntity(Entity* ent)
{
	m_EntityMap.erase(m_EntityMap.find(ent->getID()));
}