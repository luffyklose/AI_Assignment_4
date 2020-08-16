#pragma once
#include "BehaviorState.h"
#include "DisplayObject.h"


class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void Update(DisplayObject* object);

	// Getters
	BehaviorState GetOuterState();
	BehaviorState GetInnerState();

	// Setters
	void SetOuterState(BehaviorState new_state);
	void SetInnerState(BehaviorState new_state);

private:
	BehaviorState m_currentOuterState;
	BehaviorState m_currentInnerState;

	// Actions
	void m_moveToLOS(DisplayObject* object);
	void m_meleeAttack(DisplayObject* object);
	void m_flee(DisplayObject* object);
};
