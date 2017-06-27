 #include "stdafx.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include <iostream>




void MessageDispatcher::Discharge(Entity* receiver, const Telegram& msg)
{
	if (!receiver->HandleMessage(msg))
	{
		std::cout << "ERROR: Message not handled";
	}

}

MessageDispatcher* MessageDispatcher::Instance()
{
	static MessageDispatcher* instance;

	return instance;

}

void MessageDispatcher::DispatchMessage(double delay, int sender, int receiver, int msg, void* extraInfo)
{
	//Entity* senderEnt = EntityManager::Instance()->GetEntity(sender);
	Entity* receiverEnt = EntityManager::Instance()->GetEntity(receiver);

	Telegram telegram(0, sender, receiver, msg, extraInfo);

	if (delay <= 0.0f)
	{
		//std::cout << "Sending message!\n";
		Discharge(receiverEnt, telegram);
	}
}

// void MessageDispatcher::DispatchDelayedMessage()
// {
// 
// }

MessageDispatcher::~MessageDispatcher()
{
}
