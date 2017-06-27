#pragma once
#include <map>
#include <string>
#include "Entity.h"

class EntityManager
{
private:

	
	///std::map med entitys och deras ID:n
	typedef std::map<int, Entity*> EntityMap;

	EntityMap m_EntityMap;

public:
	EntityManager();
	~EntityManager();

	///Singleton
	static EntityManager* Instance();

	///Lägger till agent i map:en
	void RegisterEntity(Entity* newEnt);

	///Hämta agent ifrån id
	Entity* GetEntity(int id);

	//Tabort entity från map
	void RemoveEntity(Entity* ent);

};

