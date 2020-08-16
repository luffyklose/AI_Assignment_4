#pragma once
#include "Sprite.h"

class FireBall :public Sprite
{
public:
	FireBall();
	~FireBall();
	
	void draw() override;
	void update() override;
	void clean() override;
	
	void reset();
	
	int getMaxhealth() override { return 0; }

	bool IsActive() { return m_bIsActive; }
	void setActive();
	void setFace(int face);

private:
	void m_buildAnimations();
	int m_speed;
	bool m_bIsActive;
	int m_damage;
	int m_face;
};
