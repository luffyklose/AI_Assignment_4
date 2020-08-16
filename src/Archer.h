#pragma once

#include "BehaviorState.h"
#include "Enemy.h"
#include "HealthBarBorder.h"
#include "HealthBarFiller.h"

class Archer final : public Enemy
{
public:
	Archer(Player* player);
	~Archer();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void reset() override;
	virtual void setActive() override;
	
	std::vector<PathConnection*> getPath();
	void getDir();

private:
	void m_buildAnimations();
	//void MovePlane();
	//void SetNextNode();
	void m_checkCurrentConditions();
	void m_stateMachineUpdate();
	void Move2NearestAttackNode();
	void Shoot();
	void setFaceDir();

public:
	
private:
	bool M_withinShootRange,
		m_attackMode;
	int m_shootCounter;

	BehaviorState m_outerState,
		m_innerState;
	
	HealthBarBorder* m_pBorder;
	HealthBarFiller* m_pFiller;
};



