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

	 void generateFireball();
	void update();
	void draw();
	void Init();
	 std::list<FireBall*> getFireBallList() { return m_pFireBallPool; }
	
private:
	ProjectileManager();
	~ProjectileManager();

	 void m_buildFireballPool();
	
	//int m_FireballNumber;
	std::list<FireBall*> m_pFireBallPool;

	static ProjectileManager* s_pInstance;	
};