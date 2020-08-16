#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "HealthBarBorder.h"
#include "HealthBarFiller.h"
#include "Sprite.h"

class DestructibleObstacle : public Sprite
{
public:
	DestructibleObstacle();
	~DestructibleObstacle();

	// DisplayObject Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual int getMaxhealth() { return ObstacleMaxHP; }
	
	void DecHP(int damage);

	void reset();
	
	bool isActive();
	void setActive();
	
private:
	const int ObstacleMaxHP = 50;
	int m_curHP;
	bool m_bIsActive;
	
	HealthBarBorder* m_pBorder;
	HealthBarFiller* m_pFiller;
};



#endif /* defined (__OBSTACLE__) */