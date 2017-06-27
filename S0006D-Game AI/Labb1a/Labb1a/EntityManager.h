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

	///L�gger till agent i map:en
	void RegisterEntity(Entity* newEnt);

	///H�mta agent ifr�n id
	Entity* GetEntity(int id);

	//Tabort entity fr�n map
	void RemoveEntity(Entity* ent);

};

