#pragma once
#include "HealthBarBorder.h"
#include "HealthBarFiller.h"
#include "PlayerAnimationState.h"
#include "PlayerState.h"
#include "Sprite.h"
#include "PathNode.h"
#include"NodeManager.h"


const int PLAYERMAXHEALTH = 200;


class Player final : public Sprite
{
public:
	Player(float x,float y);
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	void setCurNode();
	void DecHP(int damage);

	// setters
	void setPlayerState(PlayerState state);
	void resetShootCounter() { m_shootCounter = 0; }
	void resetMeleeCounter() { m_meleeCounter = 0; }

	//getters
	int getMeleeDamage();
	int getRangeDamage();
	virtual int getMaxhealth() { return PLAYERMAXHEALTH; }
	PathNode* getCurNode() { return m_curNode; }
	PlayerState getCurState() { return m_currentState; }

	void m_checkCurrentConditions();
	void resetHitRecover() { m_hitRecoverCounter = 0; }
	int getMeleeRange() { return m_meleeRange; }
	
	bool canMelee();
	bool canShoot();

private:
	void m_buildAnimations();

	PlayerState m_currentState;

	HealthBarBorder* m_pBorder;
	HealthBarFiller* m_pFiller;
	PathNode* m_curNode;

	int m_hitRecoverCounter;
	const int m_meleeRange;
	
	int m_meleeCounter,
		m_shootCounter;
};