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
	void RemoveInvalid();
	void exit();
	void addFireBall2PlayerVec(FireBall* fireball);
	void addFireBall2EnemyVec(FireBall* fireball);

	//getter
	std::list<FireBall*> getFireBallList() { return m_pFireBallPool; }
	std::vector<FireBall*> getPlayerFireVec() { return m_playerFireVec; }
	std::vector<FireBall*> getEnemyFireVec() { return m_enemyFireVec; }
private:
	ProjectileManager();
	~ProjectileManager();

	 void m_buildFireballPool();
	
	//int m_FireballNumber;
	std::list<FireBall*> m_pFireBallPool;
	std::vector<FireBall*> m_playerFireVec;
	std::vector<FireBall*> m_enemyFireVec;

	static ProjectileManager* s_pInstance;	
};