#include "StateMachine.h"

StateMachine::StateMachine() : m_currentOuterState(FIGHT), m_currentInnerState(PATROL)
{
}

StateMachine::~StateMachine()
= default;

void StateMachine::Update(DisplayObject * object)
{
	switch (m_currentOuterState)
	{
	case FIGHT:
		switch (m_currentInnerState)
		{
		case PATROL:
			// Patrol Action
			break;
		case MELEE_ATTACK:
			// Perform Melee Attack Action
			m_meleeAttack(object);
			break;
		case RANGED_ATTACK:
			// Perform Ranged Attack Action
			break;
		case MOVE_TO_LOS:
			// Move 2 LOS Action
			m_moveToLOS(object);
			break;
		case MOVE_TO_MELEE:
			// Move 2 Melee Range Action
			break;
		case MOVE_TO_RANGED:
			// Move 2 Ranged Position Action
			break;
		case MOVE_TO_COVER:
			// Move 2 Cover Action
			break;
		case WAIT_IN_COVER:
			// Wait In cover Action
			break;
		}

		break;
	case FLIGHT:
		// Flee Action
		m_flee(object);
		break;
	}
}

BehaviorState StateMachine::GetOuterState()
{
	return m_currentOuterState;
}

BehaviorState StateMachine::GetInnerState()
{
	return m_currentInnerState;
}

void StateMachine::SetOuterState(BehaviorState new_state)
{
	m_currentOuterState = new_state;
}

void StateMachine::SetInnerState(BehaviorState new_state)
{
	m_currentInnerState = new_state;
}

// ACTIONS

void StateMachine::m_moveToLOS(DisplayObject * object)
{
	// get nearest node with LOS
}

void StateMachine::m_meleeAttack(DisplayObject * object)
{
	// object performs melee attack with animation
}

void StateMachine::m_flee(DisplayObject * object)
{
	// object flees
}
