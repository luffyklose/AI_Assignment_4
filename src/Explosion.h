#pragma once
#include "DisplayObject.h"
#include "Sprite.h"

class Explosion : public Sprite
{
public:
	Explosion();
	~Explosion();

	void draw() override;
	void update() override;
	void clean() override;

	void reset();

	//gettts and setters
	bool isActive();
	void setActive();
	
private:
	void m_buildAnimations();
	int getMaxhealth() override;
private:
	//int m_currentFrame;
	bool m_bIsActive;

};
