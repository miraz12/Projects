#include "EntityStates.h"
#include "State.h"
#include "Scout.h"
#include "Locations.h"
#include <iostream>
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityManager.h"
#include "AStar.h"
#include "NavNode.h"
#include "NavEdge.h"
#include "SparseGraph.h"
#include "Worker.h"
#include <time.h>
#include <random>
#include "Builder.h"
#include "Kilner.h"
#include "SettingsLoader.h"


///////////////Mining state///////////////////////////////////////////////////////


void ScoutMap::StateEnter(Scout* worker)
{
	std::cout << "Exploring the map!\n";
}

void ScoutMap::Execute(Scout* worker)
{
	
	if (worker->getPathToTarget().size() == 0)
	{
		int ranX = 0;
		int ranS = 0;
		while (worker->tempGraph->getNode(ranX).getFlag() == unwalkable)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 10000);
			ranX = dis(gen);
		}

		std::cout << ranX << "\n";
		AStar search(*worker->tempGraph, worker->getLocationNode(), worker->tempGraph->getNode(ranX));

		worker->setPathToTarget(search.GetPathToTarget());
		int size = worker->getPathToTarget().size();
	}
	else
	{
		bool seeking = false;
		int i = worker->getPathToTarget().front();

		if (glm::distance(worker->tempGraph->getNode(i).getPos(), worker->Pos()) < 1.0f)
		{
			//i -= 1;
			worker->setLocationNode(worker->tempGraph->getNode(i));
			worker->getPathToTarget().pop_front();

			worker->getMgr()->addVisitedNode(i);

			for (int j = i - 101; j <= i - 99; j++)
			{
				if (!(worker->tempGraph->getNode(j).getType() == mountain || worker->tempGraph->getNode(j).getType() == water))
				{
					worker->getMgr()->addVisitedNode(j);
				}

			}
			for (int k = i - 1; k <= i + 1; k++)
			{
				if (!(worker->tempGraph->getNode(k).getType() == mountain || worker->tempGraph->getNode(k).getType() == water))
				{
					worker->getMgr()->addVisitedNode(k);
				}

			}
			for (int l = i + 99; l <= i + 101; l++)
			{
				if (!(worker->tempGraph->getNode(l).getType() == mountain || worker->tempGraph->getNode(l).getType() == water))
				{
					worker->getMgr()->addVisitedNode(l);
				}

			}
			seeking = false;
		}
		else if (seeking == false)
		{
			worker->setNexPos(worker->tempGraph->getNode(i).getPos());
			seeking = true;
		}
			
		}
}

void ScoutMap::StateExit(Scout* worker)
{
	
}

ScoutMap* ScoutMap::Instance()
{
	static ScoutMap instance;

	return (&instance);
}

bool ScoutMap::OnMessage(Scout* worker, const Telegram& msg)
{
	return true;
}

///////////////Mining state///////////////////////////////////////////////////////
void SearchTree::StateEnter(Worker* worker)
{
	std::cout << "Are threre any trees around?\n";
}

void SearchTree::Execute(Worker* worker)
{
	if (EntityManager::Instance()->getScouts() < SettingsLoader::Instance()->getNumberOfScouts())
	{
		worker->getStateMachine()->ChangeState(UppgradeToScout::Instance());
		EntityManager::Instance()->getScouts() += 1;
	}

	if (EntityManager::Instance()->getBuilder() < SettingsLoader::Instance()->getNumberOfBuilders())
	{
		worker->getStateMachine()->ChangeState(UppgradeToBuilder::Instance());
		EntityManager::Instance()->getBuilder() += 1;
	}
	else if (EntityManager::Instance()->getKilner() < SettingsLoader::Instance()->getNumberOfKilners())
	{
		worker->getStateMachine()->ChangeState(UppgradeToKilner::Instance());
		EntityManager::Instance()->getKilner() += 1;

	}



	else
	{
		for (std::list<int>::iterator it = worker->getVisIndex()->begin(); it != worker->getVisIndex()->end(); it++)
		{
			int x = worker->tempGraph->getNode(*it).getType();
			if (worker->tempGraph->getNode(*it).getType() == tree && worker->tempGraph->getNode(*it).getFlag() == walkable)
			{
				std::cout << "Lookin for trees!\n";
				worker->tempGraph->getNode(worker->getLocationNode().getIndex()).setOccupied(true);
				int i = worker->getLocationNode().getIndex();
				AStar search(*worker->tempGraph, worker->getLocationNode(), worker->tempGraph->getNode(*it));
				std::list<int> p = search.GetPathToTarget();
				worker->setPathToTarget(search.GetPathToTarget());
				worker->getStateMachine()->ChangeState(WorkerMove::Instance());
				return;
			}
			worker->setVelocity(glm::vec2(0.0f, 0.0f));
		}
	}
	
}

void SearchTree::StateExit(Worker* worker)
{

}

SearchTree* SearchTree::Instance()
{
	static SearchTree instance;

	return (&instance);
}

bool SearchTree::OnMessage(Worker* worker, const Telegram& msg)
{
	return true;
}

///////////////Mining state///////////////////////////////////////////////////////
void ChopTree::StateEnter(Worker* worker)
{
	std::cout << "Gonna chop that tree down!\n";


}

void ChopTree::Execute(Worker* worker)
{
	if (worker->tempGraph->getNode(worker->getLocationNode().getIndex()).getType() == tree)
	{

			if (worker->tempGraph->getNode(worker->getLocationNode().getIndex()).chopWood())
			{

				worker->getStateMachine()->ChangeState(ChopChop::Instance());
				

			}
			else
			{
				std::cout << "No trees left!\n";

				worker->getStateMachine()->ChangeState(SearchTree::Instance());
			}
		


	}
	else
	{
		std::cout << "No trees left!\n";

		worker->getStateMachine()->ChangeState(SearchTree::Instance());
	}
	
	
	
}

void ChopTree::StateExit(Worker* worker)
{
	
}

ChopTree* ChopTree::Instance()
{
	static ChopTree instance;

	return (&instance);
}

bool ChopTree::OnMessage(Worker* worker, const Telegram& msg)
{
	return true;
}
///////////////Mining state///////////////////////////////////////////////////////
void ChopChop::StateEnter(Worker* worker)
{
	std::cout << "Gonna chop that tree down!\n";
	worker->getStartTime() = glfwGetTime();;


}

void ChopChop::Execute(Worker* worker)
{


	if (glfwGetTime() - worker->getStartTime() >= SettingsLoader::Instance()->getChopTreeTime())
	{
		//worker->tempGraph->getNode(worker->getLocationNode().getIndex()).chop();
		worker->setWood(1);
		worker->setHandsFull(true);
		worker->getLocationNode().getNodeTiles().pop_back();
		std::cout << "Got som wood!\n";
		worker->tempGraph->getNode(worker->getLocationNode().getIndex()).setOccupied(false);
		worker->getStateMachine()->ChangeState(TransportToBase::Instance());
	}
	else
	{
		worker->setVelocity(glm::vec2(0.0f, 0.0f));
	}
			
			



}

void ChopChop::StateExit(Worker* worker)
{

}

ChopChop* ChopChop::Instance()
{
	static ChopChop instance;

	return (&instance);
}

bool ChopChop::OnMessage(Worker* worker, const Telegram& msg)
{
	return true;
}
///////////////Mining state///////////////////////////////////////////////////////

void IdleWorker::StateEnter(Worker* worker)
{
	std::cout << "Not doing anything\n";
}


void IdleWorker::Execute(Worker* worker)
{

	worker->getNexPos() = worker->Pos();
}

void IdleWorker::StateExit(Worker* worker)
{


}

IdleWorker* IdleWorker::Instance()
{
	static IdleWorker instance;

	return (&instance);
}

bool IdleWorker::OnMessage(Worker* worker, const Telegram& msg)
{
	return true;
}

///////////////Mining state///////////////////////////////////////////////////////

void WorkerMove::StateEnter(Worker* worker)
{
	std::cout << "Moving to target\n";
	//worker->tempGraph = worker->getPath()->getGraph();
	
}

void WorkerMove::Execute(Worker* worker)
{

	if (worker->getPathToTarget().size() != 0)
	{
		
		int i = worker->getPathToTarget().front();
		if (glm::distance(worker->tempGraph->getNode(i).getPos(), worker->Pos()) < 1.0f)
		{
			worker->setLocationNode(worker->tempGraph->getNode(i));
			worker->getPathToTarget().pop_front();
		}
		else
		{
			worker->setNexPos(worker->tempGraph->getNode(i).getPos());
		}
	}
	else
	{
		if (worker->getLocationNode().getIndex() == worker->getStartNode().getIndex()+1 && worker->getHandsFull())
		{
			
			//EntityManager::Instance()->leaveWoodAtBase();

			worker->tempGraph->getNode(worker->getLocationNode().getIndex()).getWood() += 1;
			int tempint = worker->tempGraph->getNode(worker->getLocationNode().getIndex()).getWood();
			worker->setHandsFull(false);
		}
		else if(!worker->getHandsFull() && worker->getLocationNode().getType() == tree){
			worker->getStateMachine()->ChangeState(ChopTree::Instance());
		}
		else  {
			worker->getStateMachine()->ChangeState(SearchTree::Instance());
		}
	}
}

void WorkerMove::StateExit(Worker* worker)
{


}

WorkerMove* WorkerMove::Instance()
{
	static WorkerMove instance;

	return (&instance);
}

bool WorkerMove::OnMessage(Worker* worker, const Telegram& msg)
{
	return true;
}
///////////////Mining state///////////////////////////////////////////////////////
void TransportToBase::StateEnter(Worker* worker)
{
	std::cout << "leaving stuff att base\n";
	
}



void TransportToBase::Execute(Worker* worker)
{
	std::cout << "Trasnporting to base\n";

	AStar search(*worker->tempGraph, worker->getLocationNode(), worker->tempGraph->getNode(worker->getStartNode().getIndex() + 1));


	worker->setPathToTarget(search.GetPathToTarget());
	worker->getStateMachine()->ChangeState(WorkerMove::Instance());
	return;
		
	
}

void TransportToBase::StateExit(Worker* worker)
{

}

TransportToBase* TransportToBase::Instance()
{
	static TransportToBase instance;

	return (&instance);
}

bool TransportToBase::OnMessage(Worker* worker, const Telegram& msg)
{
	return true;
}
///////////////Mining state///////////////////////////////////////////////////////
void BuildCharcoalKiln::StateEnter(Builder* worker)
{
	worker->getStartTime() = glfwGetTime();
}
void BuildCharcoalKiln::Execute(Builder* worker)
{
	worker->Pos() = worker->getLocationNode().getPos();
	worker->getNexPos() = worker->Pos();
	worker->setVelocity(glm::vec2(0.0f, 0.0f));

	if (worker->getLocationNode().getIndex() != worker->getStartNode().getIndex()+1)
	{
		AStar search(*worker->tempGraph, worker->getLocationNode(), worker->tempGraph->getNode(worker->getStartNode().getIndex() + 1));

		worker->setPathToTarget(search.GetPathToTarget());
		worker->getStateMachine()->ChangeState(MoveToBuildSite::Instance());
		return;
	}
	else
	{
		if (glfwGetTime() - worker->getStartTime() > SettingsLoader::Instance()->getBuildKilnTime() && worker->tempGraph->getNode(worker->getLocationNode().getIndex()).getWood() >= 10)
		{
			worker->tempGraph->getNode(worker->getLocationNode().getIndex()).getWood() -= 10;
			worker->setNexPos(worker->getLocationNode().getPos());
			worker->tempGraph->getNode(worker->getLocationNode().getIndex()).setType(charcoalKiln);
			EntityManager::Instance()->addbuildingPos(worker->getLocationNode().getIndex());
		}
		else
		{
			worker->setVelocity(glm::vec2(0.0f, 0.0f));
		}


	}
}

void BuildCharcoalKiln::StateExit(Builder* worker)
{

}

BuildCharcoalKiln* BuildCharcoalKiln::Instance()
{
	static BuildCharcoalKiln instance;

	return (&instance);
}

bool BuildCharcoalKiln::OnMessage(Builder* worker, const Telegram& msg)
{
	return true;
}
///////////////Mining state///////////////////////////////////////////////////////
void MoveToBuildSite::StateEnter(Builder* worker)
{
	std::cout << "Moving to build site\n";

}
void MoveToBuildSite::Execute(Builder* worker)
{
	if (worker->getPathToTarget().size() != 0)
	{

		int i = worker->getPathToTarget().front();
		if (glm::distance(worker->tempGraph->getNode(i).getPos(), worker->Pos()) < 0.5f)
		{
			worker->setLocationNode(worker->tempGraph->getNode(i));
			worker->getPathToTarget().pop_front();
		}
		else
		{
			worker->setNexPos(worker->tempGraph->getNode(i).getPos());
		}
	}
	else
	{
		worker->setNexPos(worker->getPos().getPos());
		worker->getStateMachine()->ChangeState(BuildCharcoalKiln::Instance());
	}
}

void MoveToBuildSite::StateExit(Builder* worker)
{

}

MoveToBuildSite* MoveToBuildSite::Instance()
{
	static MoveToBuildSite instance;

	return (&instance);
}

bool MoveToBuildSite::OnMessage(Builder* worker, const Telegram& msg)
{
	return true;
}
///////////////Mining state///////////////////////////////////////////////////////
void WorkAtKiln::StateEnter(Kilner* worker)
{
	std::cout << "Moving to build site\n";
	worker->getStartTime() = glfwGetTime();

}
void WorkAtKiln::Execute(Kilner* worker)
{

	if (worker->getLocationNode().getType() != charcoalKiln)
	{
		for (int i = 0; i < EntityManager::Instance()->getBuildingPositions().size(); i++)
		{
			int a = EntityManager::Instance()->getBuildingPositions()[i];
			if (worker->tempGraph->getNode(EntityManager::Instance()->getBuildingPositions()[i]).getType() == charcoalKiln)
			{
				AStar search(*worker->tempGraph, worker->getLocationNode(), worker->tempGraph->getNode(EntityManager::Instance()->getBuildingPositions()[i]));

				worker->setPathToTarget(search.GetPathToTarget());
				worker->getStateMachine()->ChangeState(MoveToKiln::Instance());

				return;
			}
		}
	}
	else
	{
		if (glfwGetTime() - worker->getStartTime() > SettingsLoader::Instance()->getBurnTreeTime())
		{
			worker->setNexPos(worker->getLocationNode().getPos());
			int tempint = worker->tempGraph->getNode(worker->getLocationNode().getIndex()).getWood();
			if (worker->tempGraph->getNode(worker->getLocationNode().getIndex()).getWood() >= 2)
			{
				worker->tempGraph->getNode(worker->getLocationNode().getIndex()).getWood() -= 2;
				EntityManager::Instance()->makeCharcoal();
				worker->getStartTime() = glfwGetTime();
			}
		}
		else
		{
			worker->setVelocity(glm::vec2(0.0f, 0.0f));
		}


	}
}

void WorkAtKiln::StateExit(Kilner* worker)
{

}

WorkAtKiln* WorkAtKiln::Instance()
{
	static WorkAtKiln instance;

	return (&instance);
}

bool WorkAtKiln::OnMessage(Kilner* worker, const Telegram& msg)
{
	return true;
}
///////////////Mining state///////////////////////////////////////////////////////
void MoveToKiln::StateEnter(Kilner* worker)
{
	std::cout << "Moving to build site\n";

}
void MoveToKiln::Execute(Kilner* worker)
{
	if (worker->getPathToTarget().size() != 0)
	{

		int i = worker->getPathToTarget().front();
		if (glm::distance(worker->tempGraph->getNode(i).getPos(), worker->Pos()) < 0.5f)
		{
			worker->setLocationNode(worker->tempGraph->getNode(i));
			worker->getPathToTarget().pop_front();
		}
		else
		{
			worker->setNexPos(worker->tempGraph->getNode(i).getPos());
		}
	}
	else
	{
		worker->getStateMachine()->ChangeState(WorkAtKiln::Instance());
	}
}

void MoveToKiln::StateExit(Kilner* worker)
{

}

MoveToKiln* MoveToKiln::Instance()
{
	static MoveToKiln instance;

	return (&instance);
}

bool MoveToKiln::OnMessage(Kilner* worker, const Telegram& msg)
{
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////

void UppgradeToScout::StateEnter(Worker* worker)
{
	worker->getStartTime() = glfwGetTime();
}

void UppgradeToScout::Execute(Worker* worker)
{
	if (glfwGetTime() - worker->getStartTime() > SettingsLoader::Instance()->getUpgradeScoutTime())
	{
		EntityManager::Instance()->uppgradeToScout(worker->getID());
	}


}

void UppgradeToScout::StateExit(Worker* worker)
{

}

UppgradeToScout* UppgradeToScout::Instance()
{
	static UppgradeToScout instance;

	return (&instance);
}

bool UppgradeToScout::OnMessage(Worker* worker, const Telegram& msg)
{
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////

void UppgradeToBuilder::StateEnter(Worker* worker)
{
	worker->getStartTime() = glfwGetTime();;
}

void UppgradeToBuilder::Execute(Worker* worker)
{
	if (glfwGetTime() - worker->getStartTime() > SettingsLoader::Instance()->getUpgradeBuilderTime())
	{
		EntityManager::Instance()->uppgradeToBuilder(worker->getID());
	}


}

void UppgradeToBuilder::StateExit(Worker* worker)
{

}

UppgradeToBuilder* UppgradeToBuilder::Instance()
{
	static UppgradeToBuilder instance;

	return (&instance);
}

bool UppgradeToBuilder::OnMessage(Worker* worker, const Telegram& msg)
{
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////

void UppgradeToKilner::StateEnter(Worker* worker)
{
	worker->getStartTime() = glfwGetTime();;
}

void UppgradeToKilner::Execute(Worker* worker)
{
	if (glfwGetTime() - worker->getStartTime() > SettingsLoader::Instance()->getUpgradeKilnerTime())
	{
		EntityManager::Instance()->uppgradeToKilner(worker->getID());
	}


}

void UppgradeToKilner::StateExit(Worker* worker)
{

}

UppgradeToKilner* UppgradeToKilner::Instance()
{
	static UppgradeToKilner instance;

	return (&instance);
}

bool UppgradeToKilner::OnMessage(Worker* worker, const Telegram& msg)
{
	return true;
}