#pragma once
#include "Telegram.h"


template <class entity_type>
///State basklass
class State
{
private:


public:
	///Körs första gången en agent kommer in i ett state
	virtual void StateEnter(entity_type* worker) = 0;

	///Körs varje update instans
	virtual void Execute(entity_type* worker) = 0;

	///Körs när en agent lämnar ett state
	virtual void StateExit(entity_type* worker) = 0;

	///Ger vad som händer i statet och det kommer ett meddelande
	virtual bool OnMessage(entity_type* worker, const Telegram& tele) = 0;

	virtual ~State(){};
};
