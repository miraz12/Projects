#pragma once
#include "Telegram.h"
#include "Locations.h"
#include "NavNode.h"
#include "Pathfinder.h"
#include "Unit.h"

///Base entity
class Entity
{
public:
	SparseGraph<NavNode, NavEdge>* tempGraph;

	Entity(int id);

	Entity(glm::vec2 pos) : vPos(pos),
		m_ID(NextValidID()),
		m_bTag(false)

	{}

	~Entity();

	///Körs varje loop
	virtual void Update(float time_elapsed) = 0;

	///Unikt id för varje agent
	int getID(){ return m_ID; };
	void setID(int id){ m_ID = id; };

	///Meddelande hantering
	virtual bool HandleMessage(const Telegram& msg) = 0;

	///ger agentens postion
	virtual location_type getLocation() = 0;


	NavNode getPos() const { return pos; }
	void setPos(NavNode val) { pos = val; }
	Pathfinder* getPath() const { return path; }
	void setPath(Pathfinder* val) { path = val; }
	std::list<int>& getPathToTarget() { return pathToTarget; }
	void setPathToTarget(std::list<int> val) { pathToTarget = val; }

	NavNode getLocationNode() const { return locationNode; }
	void setLocationNode(NavNode val) { locationNode = val; }

	NavNode getStartNode() const { return startNode; }
	void setStartNode(NavNode val) { startNode = val; }
	glm::vec2 Pos() const { return vPos; }
	glm::vec2& getNexPos() { return nexPos; }
	void setNexPos(glm::vec2 val) { nexPos = val; }
	Unit* getSprite() const { return sprite; }
	void setSprite(Unit* val) { sprite = val; }

	void SetID(int val){ m_ID = val; };

private:
	int m_ID;
	int m_nextValidID;
	glm::vec2 nexPos;

	//Pointer to object that is drawn out
	Unit* sprite;

	int NextValidID(){ static int NextID = 0; return NextID++; }

	Pathfinder* path;
	std::list<int> pathToTarget;
	NavNode locationNode;
	NavNode startNode;

	NavNode pos;
	

protected:

	//its location in the environment
	glm::vec2 vPos;

	//this is a generic flag. 
	bool m_bTag;

};

