#pragma once

#include "Enemy.h"
#include "HealthBarBorder.h"
#include "HealthBarFiller.h"

class Warrior final : public Enemy
{
public:
	Warrior();
	~Warrior();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void reset() override;
	virtual void setActive() override;
	
	//void setActive() override;
	
	std::vector<PathConnection*> getPath();
	void getDir();
	
private:
	void m_buildAnimations();
	void MovePlane();
	void SetNextNode();
	
private:
	HealthBarBorder* m_pBorder;
	HealthBarFiller* m_pFiller;
};

