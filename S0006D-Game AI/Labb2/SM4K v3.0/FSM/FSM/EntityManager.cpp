#include "stdafx.h"
#include "EntityManager.h"
#include "Scout.h"
#include "Kilner.h"
#include "Builder.h"
#include "shapeGenerators.hpp"
#include "loadShaders.h"
#include "SettingsLoader.h"


EntityManager::EntityManager()
{
	//addNodesToGraph(pathRef->getStart());
	woodAtBase = 0;
	charcoal = 0;
}


void EntityManager::uppgradeToScout(int id)
{
	Unit* squariObject = GetEntity(id)->getSprite();
	
	
	RemoveEntity(GetEntity(id));

	Scout* guy = new Scout(glm::vec2(0.0f, 0.0f), 2.0f, 2.0f, SettingsLoader::Instance()->getentMaxVelocity());
	guy->SetID(id);
	RegisterEntity(guy);
	guy->getStateMachine()->ChangeStateNotWorking(ScoutMap::Instance());
	guy->Update(0);

	squariObject->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	GetEntity(id)->setSprite(squariObject);



	//scouts++;
}
void EntityManager::uppgradeToBuilder(int id)
{
	Unit* squariObject = GetEntity(id)->getSprite();

	RemoveEntity(GetEntity(id));

	Builder* guy = new Builder(glm::vec2(0.0f, 0.0f), 2.0f, 2.0f, SettingsLoader::Instance()->getentMaxVelocity());
	guy->SetID(id);
	RegisterEntity(guy);
	guy->getStateMachine()->ChangeStateNotWorking(BuildCharcoalKiln::Instance());
	//guy->Update(0);
	squariObject->setColor(glm::vec3(1.0f, 1.0f, 0.0f));

	GetEntity(id)->setSprite(squariObject);

}
void EntityManager::uppgradeToKilner(int id)
{
	Unit* squariObject = GetEntity(id)->getSprite();

	RemoveEntity(GetEntity(id));

	Kilner* guy = new Kilner(glm::vec2(0.0f, 0.0f), 2.0f, 2.0f, SettingsLoader::Instance()->getentMaxVelocity());
	guy->SetID(id);
	RegisterEntity(guy);
	guy->getStateMachine()->ChangeStateNotWorking(WorkAtKiln::Instance());
	//guy->Update(0);

	squariObject->setColor(glm::vec3(0.0f, 0.5f, 1.0f));
	GetEntity(id)->setSprite(squariObject);
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

bool EntityManager::ValidNeighbour(int x, int y, int NumCellsX, int NumCellsY)
{
	return !((x < 0) || (x >= NumCellsX) || (y < 0) || (y >= NumCellsY));
}

void EntityManager::addNodesToGraph(NavNode i)
{
	visitedGraph->getNode(i.getIndex()).setFlag(walkable);


		NavNode* tempNode = &visitedGraph->getNode(i.getIndex());
		float col = tempNode->getPos().x;
		float row = tempNode->getPos().y;
		int tempNodeFlag = tempNode->getFlag();

		for (int j = -1; j < 2; ++j)
		{
			for (int k = -1; k < 2; ++k)
			{
				float nodeX = col + k;
				float nodeY = row + j;

				if ((j == 0) && (k == 0)) continue;
				


				if (ValidNeighbour(nodeX, nodeY, graphWidth, graphHeight) && tempNodeFlag == walkable)
				{
					//calculate the distance to this node
					glm::vec2 PosNode = visitedGraph->getNode(row*graphWidth + col).getPos();
					glm::vec2 PosNeighbour = visitedGraph->getNode(nodeY*graphWidth + nodeX).getPos();
					 int tempStuff = visitedGraph->getNode(nodeY*graphWidth + nodeX).getFlag();


					 if (tempStuff == unwalkable)
					{

					}
					else
					{
						double dist = glm::distance(PosNode, PosNeighbour);

						if (tempStuff == marsh)
						{
							dist *= 2;
						}
						//this neighbour is okay so it can be added
						NavEdge NewEdge(row*graphWidth + col, nodeY*graphWidth + nodeX, dist);
						NewEdge.setCost(dist);
						visitedGraph->AddEdge(NewEdge);

						//Not directed add edges both ways.
						//NavEdge NewEdgeBack(nodeY*graphWidth + nodeX, row*graphWidth + col, dist);
						//NewEdgeBack.setCost(dist);
						//visitedGraph->AddEdge(NewEdgeBack);

					}
				}
			}
		

	}
}
