#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "HealthBarBorder.h"
#include "HealthBarFiller.h"
#include "PlayerAnimationState.h"
#include "Sprite.h"
#include "PathNode.h"
#include"NodeManager.h"

const int PLAYERMAXHEALTH = 200;
const int PLAYERRANGEDAMAGE = 20;
const int PLAYERMELEEDAMAGE = 30;

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
	void setAnimationState(PlayerAnimationState new_state);

	//getters
	int getMeleeDamage();
	int getRangeDamage();
	virtual int getMaxhealth() { return PLAYERMAXHEALTH; }
	PathNode* getCurNode() { return m_curNode; }

private:
	void m_buildAnimations();

	PlayerAnimationState m_currentAnimationState;

	HealthBarBorder* m_pBorder;
	HealthBarFiller* m_pFiller;
	PathNode* m_curNode;
};

#endif /* defined (__PLAYER__) */