#include "EntityStates.h"
#include "State.h"
#include "Workers.h"
#include "Locations.h"
#include <iostream>
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityManager.h"


///////////////Mining state///////////////////////////////////////////////////////


void GoMining::StateEnter(Workers* worker)
{
	if (occupied)
	{
		std::cout << "The mine looks occupied, guess I'll go to the forest instead.\n";
		worker->getStateMachine()->ChangeStateNotWorking(GoLogging::Instance());
	}

	else if (worker->getLocation() != goldmine)
	{
		occupied = true;
		std::cout << "Walking to the mine.\n";
		worker->ChangeLocation(goldmine);
	}
}

void GoMining::Execute(Workers* worker)
{
	if (worker->Hungry())
	{
		std::cout << "Im hungry! Time to eat\n";
		worker->getStateMachine()->ChangeState(EatAtBar::Instance());
	}
	else if (worker->IsThirsty())
	{
		std::cout << "Feeling thirsty\n";
		worker->getStateMachine()->ChangeState(GoToWell::Instance());
	}

	else if (worker->IsTiered())
	{
		std::cout << "Feeling tired\n";
		worker->getStateMachine()->ChangeState(GoHome::Instance());
	}

	else if (worker->PocketsFull())
	{
		std::cout << "Cant carry more!\n";
		worker->getStateMachine()->ChangeState(GoToMarket::Instance());
	}
	else
	{
		std::cout << "mining stuff\n";
		worker->AddToGoldCarried(1);
		std::cout << "got a nööögget\n" << "ive got " << worker->getGold() << " nuggets\n";
		worker->DecreaseFatigue();
	}
}

void GoMining::StateExit(Workers* worker)
{
	occupied = false;
	std::cout << "gettin out of this mine!\n";
}

GoMining* GoMining::Instance()
{
	static GoMining instance;

	return (&instance);
}

bool GoMining::OnMessage(Workers* worker, const Telegram& msg)
{
	if (worker->Lonely())
	{
		std::cout << worker->getID() <<": Alright ill meat you there!\n";

		switch (msg.Msg)
		{
		case Msg_GoingToMarket:
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;

		case Msg_goingToTheBar:
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;

		case Msg_goingToTheWell:
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;

		default:
			return true;
		}
	}

	else if (!worker->Hungry() && !worker->IsThirsty() && !worker->IsTiered())
	{
		std::cout << worker->getID() << ": Alright ill meat you there!\n";

		switch (msg.Msg)
		{
		case Msg_GoingToMarket:
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;

		case Msg_goingToTheBar:
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;

		case Msg_goingToTheWell:
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;

		default:
			return true;
		}
	}

	else
	{
		if (worker->Hungry() && msg.Msg == Msg_GoingToMarket)
		{
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;
		}
		else if (worker->Hungry() && msg.Msg == Msg_goingToTheBar)
		{
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;
		}
		else if (worker->Hungry() && msg.Msg == Msg_goingToTheWell)
		{
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;
		}
		else
		{
			return true;
		}
	}
}




/////////////////Go to market state////////////////////////////////////////

GoToMarket* GoToMarket::Instance()
{
	static GoToMarket instance;

	return (&instance);
}

void GoToMarket::StateEnter(Workers* worker)
{
	if (worker->getLocation() != market)
	{
		std::cout << "Walking to the market.\n";
		worker->ChangeLocation(market);
	}
}

void GoToMarket::Execute(Workers* worker)
{

	if (worker->Depressed())
	{
		std::cout << "Hey, anyone wanna meat up at the market?\n";
		for (int i = 1; i <= 4; i++)
		{
			if (i != worker->getID())
			{
				MessageDispatcher::Instance()->DispatchMessage(0, worker->getID(), i, Msg_GoingToMarket, 0);
			}
		}
	}

	for (int i = 1; i <= 4; i++)
	{
		if (i != worker->getID())
		{
			if (EntityManager::Instance()->GetEntity(i)->getLocation() == Msg_GoingToMarket)
			{
				std::cout << "Good talk!\n";
				worker->BoostMorale();
			}
		}
	}

	if (worker->Hungry())
	{
		std::cout << "Im hungry! Time to eat\n";
		worker->getStateMachine()->ChangeState(EatAtBar::Instance());
	}
	else if (worker->IsThirsty())
	{
		std::cout << "Feeling thirsty\n";
		worker->getStateMachine()->ChangeState(GoToWell::Instance());
	}


	else if (worker->IsTiered())
	{
		std::cout << "Feeling tired\n";
		worker->getStateMachine()->ChangeState(GoHome::Instance());
	}
	else
	{
		std::cout << "Trading goods \n";
		worker->SellGoods();
		std::cout << "Got " << worker->getCurrency() << " coins\n";
		worker->getStateMachine()->RevertToPrevious();
	}

}

void GoToMarket::StateExit(Workers* worker)
{
	std::cout << "leaving the market\n";
}

bool GoToMarket::OnMessage(Workers* worker, const Telegram& msg)
{
	if (msg.Msg == Msg_GoingToMarket)
	{
		return true;
	}
	if (worker->Lonely())
	{
		std::cout << worker->getID() << ": Alright ill meat you there!\n";

		switch (msg.Msg)
		{
		case Msg_GoingToMarket:
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;

		case Msg_goingToTheBar:
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;

		case Msg_goingToTheWell:
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;

		default:
			return true;
		}
	}

	else if (!worker->Hungry() && !worker->IsThirsty() && !worker->IsTiered())
	{
		std::cout << worker->getID() << ": Alright ill meat you there!\n";

		switch (msg.Msg)
		{
		case Msg_GoingToMarket:
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;

		case Msg_goingToTheBar:
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;

		case Msg_goingToTheWell:
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;

		default:
			return true;
		}

	}

	else
	{
		if (worker->Hungry() && msg.Msg == Msg_GoingToMarket)
		{
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;
		}
		else if (worker->Hungry() && msg.Msg == Msg_goingToTheBar)
		{
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;
		}
		else if (worker->Hungry() && msg.Msg == Msg_goingToTheWell)
		{
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;
		}
		else
		{
			return true;
		}
	}
}

/////////////////GoToWell////////////////////////////////////////

GoToWell* GoToWell::Instance()
{
	static GoToWell instance;

	return &instance;
}

void GoToWell::StateEnter(Workers* worker)
{
	if (worker->getLocation() != well)
	{
		worker->ChangeLocation(well);
	}
	std::cout << "Time to go drink at the well\n";

}

void GoToWell::Execute(Workers* worker)
{
	if (worker->Depressed())
	{
		std::cout << "Hey, anyone wanna meat up at the well?\n";
		for (int i = 1; i <= 4; i++)
		{
			if (i != worker->getID())
			{
				MessageDispatcher::Instance()->DispatchMessage(0, worker->getID(), i, Msg_goingToTheWell, 0);
			}
		}
	}

	for (int i = 1; i <= 4; i++)
	{
		if (i != worker->getID())
		{
			if (EntityManager::Instance()->GetEntity(i)->getLocation() == well)
			{
				std::cout << "Good talk!\n";
				worker->BoostMorale();
			}
		}
	}
	if (worker->IsThirsty())
	{

		std::cout << "MMMMHH TASTY WAAATERRRERERERER!\n";
		worker->Drink();

		//worker->getStateMachine()->RevertToPrevious();
	}

	else
	{
		worker->getStateMachine()->ChangeState(GoMining::Instance());
		//worker->getStateMachine()->RevertToPrevious();
	}
}

void GoToWell::StateExit(Workers* worker)
{
	std::cout << "Leaving the well\n";
}

bool GoToWell::OnMessage(Workers* worker, const Telegram& msg)
{
	if (msg.Msg == Msg_goingToTheWell)
	{
		return true;
	}

	if (worker->Lonely())
	{
		std::cout << worker->getID() << ": Alright ill meat you there!\n";

		switch (msg.Msg)
		{
		case Msg_GoingToMarket:
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;

		case Msg_goingToTheBar:
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;

		case Msg_goingToTheWell:
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;

		default:
			return true;
		}
	}

	else if (!worker->Hungry() && !worker->IsThirsty() && !worker->IsTiered())
	{
		std::cout << worker->getID() << ": Alright ill meat you there!\n";

		switch (msg.Msg)
		{
		case Msg_GoingToMarket:
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;

		case Msg_goingToTheBar:
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;

		case Msg_goingToTheWell:
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;

		default:
			return true;
		}
	}

	else
	{
		if (worker->Hungry() && msg.Msg == Msg_GoingToMarket)
		{
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;
		}
		else if (worker->Hungry() && msg.Msg == Msg_goingToTheBar)
		{
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;
		}
		else if (worker->Hungry() && msg.Msg == Msg_goingToTheWell)
		{
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;
		}
		else
		{
			return true;
		}
	}
}

////////////////////////GoLogging//////////////////////////////////////////////

GoLogging* GoLogging::Instance()
{
	static GoLogging instance;

	return (&instance);
}
void GoLogging::StateEnter(Workers* worker)
{

	if (occupied)
	{
		std::cout << "The forset looks occupied, guess I'll go to the forest instead.\n";
		worker->getStateMachine()->ChangeStateNotWorking(GoFishing::Instance());
	}

	else if (worker->getLocation() != forest)
	{
		occupied = true;
		std::cout << "Walking to the forest.\n";
		worker->ChangeLocation(forest);
	}
}

void GoLogging::Execute(Workers* worker)
{
	if (worker->Hungry())
	{
		std::cout << "Im hungry! Time to eat\n";
		worker->getStateMachine()->ChangeState(EatAtBar::Instance());
	}
	else if (worker->IsThirsty())
	{
		std::cout << "Feeling thirsty\n";
		worker->getStateMachine()->ChangeState(GoToWell::Instance());
	}


	else if (worker->IsTiered())
	{
		std::cout << "Feeling tired\n";
		worker->getStateMachine()->ChangeState(GoHome::Instance());
	}

	else if (worker->PocketsFull())
	{
		std::cout << "Cant carry more!\n";
		worker->getStateMachine()->ChangeState(GoToMarket::Instance());
	}
	else
	{
		std::cout << "Chopping wood\n";
		worker->AddToWoodCarried(1);
		std::cout << "got some wood\n" << "ive got " << worker->getWood() << " loggs\n";
		worker->DecreaseFatigue();
	}
}

void GoLogging::StateExit(Workers* worker)
{
	occupied = false;
	std::cout << "leaving the the forest\n";
}

bool GoLogging::OnMessage(Workers* worker, const Telegram& msg)
{
	if (worker->Lonely())
	{
		std::cout << worker->getID() << ": Alright ill meat you there!\n";

		switch (msg.Msg)
		{
		case Msg_GoingToMarket:
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;

		case Msg_goingToTheBar:
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;

		case Msg_goingToTheWell:
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;

		default:
			return true;
		}
	}

	else if (!worker->Hungry() && !worker->IsThirsty() && !worker->IsTiered())
	{
		std::cout << worker->getID() << ": Alright ill meat you there!\n";

		switch (msg.Msg)
		{
		case Msg_GoingToMarket:
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;

		case Msg_goingToTheBar:
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;

		case Msg_goingToTheWell:
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;

		default:
			return true;
		}

	}

	else
	{
		if (worker->Hungry() && msg.Msg == Msg_GoingToMarket)
		{
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;
		}
		else if (worker->Hungry() && msg.Msg == Msg_goingToTheBar)
		{
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;
		}
		else if (worker->Hungry() && msg.Msg == Msg_goingToTheWell)
		{
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;
		}
		else
		{
			return true;
		}
	}
}

////////////////////////GoFishing//////////////////////////////////////////////

GoFishing* GoFishing::Instance()
{
	static GoFishing instance;

	return &instance;
}

void GoFishing::StateEnter(Workers* worker)
{
	if (occupied)
	{
		std::cout << "The fishing pond looks occupied, guess I'll become an outlaw.\n";
		worker->getStateMachine()->ChangeStateNotWorking(WorkAtMarket::Instance());
	}

	else if (worker->getLocation() != fishingPond)
	{
		occupied = true;
		std::cout << "Walking to the fishing pond.\n";
		worker->ChangeLocation(fishingPond);
	}
}

void GoFishing::Execute(Workers* worker)
{
	if (worker->Hungry())
	{
		std::cout << "Im hungry! Time to eat\n";
		worker->getStateMachine()->ChangeState(EatAtBar::Instance());
	}
	else if (worker->IsThirsty())
	{
		std::cout << "Feeling thirsty\n";
		worker->getStateMachine()->ChangeState(GoToWell::Instance());
	}


	else if (worker->IsTiered())
	{
		std::cout << "Feeling tired\n";
		worker->getStateMachine()->ChangeState(GoHome::Instance());
	}

	else if (worker->PocketsFull())
	{
		std::cout << "Cant carry more!\n";
		worker->getStateMachine()->ChangeState(GoToMarket::Instance());
	}
	else
	{

		std::cout << "fishing\n";
		worker->AddToFishCarried(1);
		std::cout << "got a fish!\n" << "ive got " << worker->getFish() << " fish\n";
		worker->DecreaseFatigue();
	}
}

void GoFishing::StateExit(Workers* worker)
{
	occupied = false;
	std::cout << "leaving the the fishing pond!\n";
}

bool GoFishing::OnMessage(Workers* worker, const Telegram& msg)
{
	return true;
}

/////////////////////////GoHome///////////////////////////////////////////

GoHome* GoHome::Instance()
{
	static GoHome instance;

	return &instance;
}

void GoHome::StateEnter(Workers* worker)
{
	if (worker->getLocation() != home)
	{
		std::cout << "Walking home.\n";
		worker->ChangeLocation(home);
	}
}

void GoHome::Execute(Workers* worker)
{
	if (worker->Hungry())
	{
		std::cout << "Im hungry! Time to eat\n";
		worker->getStateMachine()->ChangeState(EatAtBar::Instance());
	}
	else if (worker->IsThirsty())
	{
		std::cout << "Feeling thirsty\n";
		worker->getStateMachine()->ChangeState(GoToWell::Instance());
	}

	else if (worker->rested())
	{
		std::cout << "Awake and ready for work\n";
		worker->getStateMachine()->ChangeState(GoMining::Instance());
	}
	else
	{
		std::cout << "Going to sleeep ZZzzZZzzzz....\n";
		worker->IncreaseFatigue();
	}
}

void GoHome::StateExit(Workers* worker)
{
	std::cout << "Leaving home.\n";
}

bool GoHome::OnMessage(Workers* worker, const Telegram& msg)
{
	return true;
}

/////////////////////////WorkAtMarket///////////////////////////////////////////

WorkAtMarket* WorkAtMarket::Instance()
{
	static WorkAtMarket instance;

	return &instance;
}

void WorkAtMarket::StateEnter(Workers* worker)
{
	if (worker->getLocation() != market)
	{
		std::cout << "Walking to the market.\n";
		worker->ChangeLocation(market);
	}
}

void WorkAtMarket::Execute(Workers* worker)
{

	if (worker->Hungry())
	{
		std::cout << "Im hungry! Time to eat\n";
		worker->getStateMachine()->ChangeState(EatAtBar::Instance());
	}
	else if (worker->IsThirsty())
	{
		std::cout << "Feeling thirsty\n";
		worker->getStateMachine()->ChangeState(GoToWell::Instance());
	}

	else if (worker->IsTiered())
	{
		std::cout << "Feeling tired\n";
		worker->getStateMachine()->ChangeState(GoHome::Instance());
	}

	else if (worker->PocketsFull())
	{
		std::cout << "Cant carry more!\n";
		worker->getStateMachine()->ChangeState(GoToMarket::Instance());
	}
	else
	{
		std::cout << "Stealing stuff at market\n";
		worker->Steal();
		std::cout << "Tried to steal som coins!\n" << "ive got " << worker->getCurrency() << " coins\n";
		worker->DecreaseFatigue();
	}
}

void WorkAtMarket::StateExit(Workers* worker)
{
	std::cout << "Leaving market.\n";
}

bool WorkAtMarket::OnMessage(Workers* worker, const Telegram& msg)
{
	if (msg.Msg == Msg_GoingToMarket)
	{
		return true;
	}
	if (worker->Lonely())
	{
		std::cout << worker->getID() << ": Alright ill meat you there!\n";

		switch (msg.Msg)
		{
		case Msg_GoingToMarket:
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;

		case Msg_goingToTheBar:
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;

		case Msg_goingToTheWell:
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;

		default:
			return true;
		}
	}

	else if (!worker->Hungry() && !worker->IsThirsty() && !worker->IsTiered())
	{
		std::cout << worker->getID() << ": Alright ill meat you there!\n";

		switch (msg.Msg)
		{
		case Msg_GoingToMarket:
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;

		case Msg_goingToTheBar:
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;

		case Msg_goingToTheWell:
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;

		default:
			return true;
		}
	}
	else
	{
		if (worker->Hungry() && msg.Msg == Msg_GoingToMarket)
		{
			worker->getStateMachine()->ChangeState(GoToMarket::Instance());
			return true;
		}
		else if (worker->Hungry() && msg.Msg == Msg_goingToTheBar)
		{
			worker->getStateMachine()->ChangeState(EatAtBar::Instance());
			return true;
		}
		else if (worker->Hungry() && msg.Msg == Msg_goingToTheWell)
		{
			worker->getStateMachine()->ChangeState(GoToWell::Instance());
			return true;
		}
		else
		{
			return true;
		}
	}
}

/////////////////////////////EatAtBar/////////////////////////////////////////

EatAtBar* EatAtBar::Instance()
{
	static EatAtBar instance;
	return &instance;
}

void EatAtBar::StateEnter(Workers* worker)
{
	if (worker->getLocation() != bar)
	{
		std::cout << "Walking to the bar.\n";
		worker->ChangeLocation(bar);
	}
}

void EatAtBar::Execute(Workers* worker)
{
	if (worker->Depressed())
	{
		std::cout << "Hey, anyone wanna meat up at the bar?\n";
		for (int i = 1; i <= 4; i++)
		{
			if (i != worker->getID())
			{
				MessageDispatcher::Instance()->DispatchMessage(0, worker->getID(), i, Msg_goingToTheBar, 0);	
			}
		}
	}

	for (int i = 1; i <= 4; i++)
	{
		if (i != worker->getID())
		{
			if (EntityManager::Instance()->GetEntity(i)->getLocation() == bar)
			{
				std::cout << "Good talk!\n";
				worker->BoostMorale();
			}
		}
	}

	if (worker->Full())
	{
		std::cout << "Aaah, Im full!\n";
		worker->getStateMachine()->ChangeState(GoMining::Instance());
	}
	else
	{
		std::cout << "Lets dig in!\n";
		worker->EatAtBar();
		worker->Drink();
	}
}

void EatAtBar::StateExit(Workers* worker)
{
	std::cout << "Leaving the bar.\n";
}

bool EatAtBar::OnMessage(Workers* worker, const Telegram& msg)
{
	return true;
}


////////////////////////Death state////////////////////////////////////

EntityDead* EntityDead::Instance()
{
	static EntityDead instance;
	return &instance;
}

void EntityDead::StateEnter(Workers* worker)
{
	std::cout << "Oh no I died!\n";
}

void EntityDead::Execute(Workers* worker)
{
	std::cout << "This entity is dead\n";
}

void EntityDead::StateExit(Workers* worker)
{

}

bool EntityDead::OnMessage(Workers* worker, const Telegram& msg)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////
