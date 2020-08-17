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

	void generateExplosion();

	void update();
	void draw();
	void Init();
	void exit();
	void RemoveInvalid();

	std::list<Explosion*>getExplosionList() { return m_pExplosionPool; }
	
private:
	ExplosionManager();
	~ExplosionManager();

	void m_buildExplosionPool();

	std::list<Explosion*> m_pExplosionPool;
	
	static ExplosionManager* s_pInstance;
};

