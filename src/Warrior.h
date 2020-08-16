#pragma once

#include "Enemy.h"
#include "HealthBarBorder.h"
#include "HealthBarFiller.h"
#include"BehaviorState.h"

class Warrior final : public Enemy
{
public:
	Warrior(Player* player);
	~Warrior();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void reset() override;
	virtual void setActive() override;
	
	//void setActive() override;
	
	//std::vector<PathConnection*> getPath();
	//void getDir();
	
private:
	void m_buildAnimations();
	void MoveWarrior();
	//void SetNextNode();
	void m_checkCurrentConditions();
	void m_stateMachineUpdate();
	void setAttackNode();
	void Move2NearestAttackNode();
	void Melee();
private:
	bool m_withinMeleeRange,
		m_attackMode;
	int m_meleeCounter;

	HealthBarBorder* m_pBorder;
	HealthBarFiller* m_pFiller;

	BehaviorState m_outerState, m_innerState;
	std::vector<PathNode*> m_NodeCanAttack;
};

