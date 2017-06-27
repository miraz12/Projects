#pragma once
#include "Telegram.h"


template <class entity_type>
///State basklass
class State
{
private:


public:
	///K�rs f�rsta g�ngen en agent kommer in i ett state
	virtual void StateEnter(entity_type* worker) = 0;

	///K�rs varje update instans
	virtual void Execute(entity_type* worker) = 0;

	///K�rs n�r en agent l�mnar ett state
	virtual void StateExit(entity_type* worker) = 0;

	///Ger vad som h�nder i statet och det kommer ett meddelande
	virtual bool OnMessage(entity_type* worker, const Telegram& tele) = 0;

	virtual ~State(){};
};
