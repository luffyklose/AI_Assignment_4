#pragma once
#include "Explosion.h"

class ExplosionManager
{
public:
	static ExplosionManager* Instance()
	{
		if (s_pInstance == nullptr)
			s_pInstance = new ExplosionManager();
		return s_pInstance;
	}

	Explosion* getExplosion();

	void update();
	void draw();
	
private:
	ExplosionManager();
	~ExplosionManager();

	void m_buildExplosionPool();

	int m_explosionNumber;
	std::list<Explosion*> m_pExplosionPool;
	
	static ExplosionManager* s_pInstance;
};

