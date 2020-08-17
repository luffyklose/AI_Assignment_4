#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "HealthBarBorder.h"
#include "HealthBarFiller.h"
#include "Sprite.h"

const int STEANMAXHP = 100;

class DestructibleObstacle : public Sprite
{
public:
	DestructibleObstacle();
	~DestructibleObstacle();

	// DisplayObject Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual int getMaxhealth() { return STEANMAXHP; }
	
	void DecHP(int damage);

	void reset();
	
	//bool isActive();
	void setActive();
	void setRow(int row) { m_row = row; }
	void setCol(int col) { m_col = col; }
	int getRow() { return m_row; }
	int getCol() { return m_col; }
	
private:
	//bool m_bIsActive;
	
	HealthBarBorder* m_pBorder;
	HealthBarFiller* m_pFiller;

	int m_row, m_col;
};



#endif /* defined (__OBSTACLE__) */