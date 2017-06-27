#pragma once
#include "State.h"

template <class entity_type>
///Håller reda på agenters states
class StateMachine
{
private:

	entity_type* m_owner;

	State<entity_type>* m_currentState;


	State<entity_type>* m_previousState;


	State<entity_type>* m_globalState;

public:
	StateMachine(entity_type* pOwner) : m_owner(pOwner),
										  m_currentState(0),
										  m_previousState(0),
										  m_globalState(0)
	{}
	~StateMachine(){};

	void setCurrentState(State<entity_type>* val) { m_currentState = val; }
	void setPreviousState(State<entity_type>* val) { m_previousState = val; }
	void setGlobalState(State<entity_type>* val) { m_globalState = val; }

	State<entity_type>* getCurrentState() const { return m_currentState; }
	State<entity_type>* getPreviousState() const { return m_previousState; }
	State<entity_type>* getGlobalState() const { return m_globalState; }

	void Update()const
	{
		if (m_globalState)
		{
			m_globalState->Execute(m_owner);
		}

		else if (m_currentState)
		{
			m_currentState->Execute(m_owner);
		}

	}

	void ChangeState(State<entity_type>* newState)
	{
		m_previousState = m_currentState;

		m_currentState->StateExit(m_owner);

		m_currentState = newState;

		m_currentState->StateEnter(m_owner);

	}

	void ChangeStateNotWorking(State<entity_type>* newState)
	{
		m_previousState = m_currentState;

		m_currentState = newState;

		m_currentState->StateEnter(m_owner);
	}

	void RevertToPrevious()
	{
		ChangeState(m_previousState);
	}

	bool isInState(const State<entity_type>& st);

	bool StateMachine::HandleMessage(const Telegram& msg)
	{
		if (m_currentState && m_currentState->OnMessage(m_owner, msg))
		{
			return true;
		}
		else if (m_globalState && m_globalState->OnMessage(m_owner, msg))
		{
			return true;
		}

		return false;
	}



};



