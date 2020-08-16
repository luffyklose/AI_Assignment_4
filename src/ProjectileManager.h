#pragma once
#include "Fireball.h"

class ProjectileManager
{
public:
	static ProjectileManager* Instance()
	{
		if (s_pInstance == nullptr)
			s_pInstance = new ProjectileManager();
		return s_pInstance;
	}

	FireBall* generateFireball();

	void update();
	void draw();

private:
	ProjectileManager();
	~ProjectileManager();

	void m_buildFireballPool();
	
	int m_FireballNumber;
	std::list<FireBall*> m_pFireBallPool;

	static ProjectileManager* s_pInstance;
	
};