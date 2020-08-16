#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "HealthBarBorder.h"
#include "HealthBarFiller.h"
#include "PlayerAnimationState.h"
#include "Sprite.h"

const int PLAYERMAXHEALTH = 200;

const int PLAYERDAMAGE = 10;

class Player final : public Sprite
{
public:
	Player(float x,float y);
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;



	// setters
	void setAnimationState(PlayerAnimationState new_state);

	//getters
	int getDamage();
	virtual int getMaxhealth() { return PLAYERMAXHEALTH; }

private:
	void m_buildAnimations();

	PlayerAnimationState m_currentAnimationState;

	const int m_playerDamage;

	HealthBarBorder* m_pBorder;
	HealthBarFiller* m_pFiller;
};

#endif /* defined (__PLAYER__) */