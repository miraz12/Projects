#pragma once
#include <map>
#include <string>
#include "Entity.h"
///Entity manager. Holds all entitys. Hive mind
class EntityManager
{
private:

	
	
	///std::map med entitys och deras ID:n
	typedef std::map<int, Entity*> EntityMap;

	EntityMap m_EntityMap;

	std::list<int> visitedNodes;
	std::list<int> visitedNodesDraw;


	SparseGraph<NavNode, NavEdge>* visitedGraph = &Pathfinder::Instance()->getDiscoveredGraph();

	Pathfinder* pathRef = Pathfinder::Instance();

	SparseGraph<NavNode, NavEdge>* grapRef =  &pathRef->getGraph();

	std::vector<int> buildingPositions;

	int graphWidth = pathRef->getGraphWidth();
	int graphHeight = pathRef->getGraphHeight();

	int woodAtBase;
	int charcoal;

	//Entitys
	int scouts = 0;
	int workers = 50;
	int builder = 0;
	int kilner = 0;

public:
	EntityManager();
	~EntityManager()
	{ 
	};

	void uppgradeToScout(int id);
	void uppgradeToBuilder(int id);
	void uppgradeToKilner(int id);


	///Singleton
	static EntityManager* Instance();

	///Lägger till agent i map:en
	void RegisterEntity(Entity* newEnt);

	///Hämta agent ifrån id
	Entity* GetEntity(int id);

	//Tabort entity från map
	void RemoveEntity(Entity* ent);

	bool ValidNeighbour(int x, int y, int NumCellsX, int NumCellsY);

	std::list<int>& getVisitedNodesDraw() { return visitedNodesDraw; }
	std::list<int>& getVisitedNodes() { return visitedNodes; }
	void addVisitedNode(int val) 
	{ 
		visitedNodes.push_back(val);
		visitedNodesDraw.push_back(val);

		//visitedNodes.unique();
		//visitedNodesDraw.unique();

		addNodesToGraph(grapRef->getNode(val));
	}

	void addNodesToGraph(NavNode n);

	SparseGraph<NavNode, NavEdge>& getVisitedGraph() { return *visitedGraph; }
	int getWoodAtBase() const { return woodAtBase; }
	void leaveWoodAtBase(){ woodAtBase++; }
	SparseGraph<NavNode, NavEdge>* getGrapRef() const { return grapRef; }
	void setGrapRef(SparseGraph<NavNode, NavEdge>* val) { grapRef = val; }
	std::list<int> getVisitedNodesDraw() const { return visitedNodesDraw; }

	EntityManager::EntityMap getEntityMap() const { return m_EntityMap; }

	std::vector<int>& getBuildingPositions() { return buildingPositions; }
	void addbuildingPos(int val) { buildingPositions.push_back(val); }

	void makeCharcoal(){ charcoal++; };
	int getCharcoal() const { return charcoal; }

	int& getScouts() { return scouts; }

	int& getWorkers() { return workers; }

	int& getBuilder() { return builder; }

	int& getKilner() { return kilner; }

};

