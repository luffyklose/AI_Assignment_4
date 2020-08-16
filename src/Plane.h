#pragma once
#ifndef __PLANE__
#define __PLANE__

#include "Enemy.h"
#include "HealthBarBorder.h"
#include "HealthBarFiller.h"
#include "StateMachine.h"
#include"BehaviorState.h"

class Plane final : public Enemy
{
public:
	Plane(float x,float y);
	~Plane();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void getDir();	
private:
	void m_buildAnimations();
	void MovePlane();
	//void SetNextNode();
	//void setPath(std::vector<PathConnection*> path);
	
	void m_checkCurrentConditions();
	void m_stateMachineUpdate();
	
public:

private:
	bool m_withinMeleeRange;

	HealthBarBorder* m_pBorder;
	HealthBarFiller* m_pFiller;

	BehaviorState m_outerState, m_innerState;
};



#endif /* defined (__PLANE__) */
