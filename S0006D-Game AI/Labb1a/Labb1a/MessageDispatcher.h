#pragma once
#include "Entity.h"
#include "Telegram.h"
#include <set>

class MessageDispatcher
{
private:
	//std::set<Telegram> PriorityQ;
	
	///Skickar meddelanden till reciver ID
	void Discharge(Entity* receiver, const Telegram& msg);

	MessageDispatcher(){};
	

public:
	
	///Singleton
	static MessageDispatcher* Instance();

	///Skapar ett meddelande för att sedan skicka det 
	void DispatchMessage(double delay, int sender, int receiver, int msg, void* extraInfo);

	//void DispatchDelayedMessage();

	~MessageDispatcher();
};

