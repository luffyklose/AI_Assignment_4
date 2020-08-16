#pragma once
#include "Archer.h"
#include "Enemy.h"
#include "Warrior.h"

class EnemyManager
{
public:
	static EnemyManager* Instance()
	{
		if (s_pInstance == nullptr)
			s_pInstance = new EnemyManager();
		return s_pInstance;
	}

	Enemy* generateArcher();
	Enemy* generateWarrior();

	std::vector<Enemy*> getEnemyVec() { return m_pEnemyVec; }
	
	void update();
	void draw();

private:
	EnemyManager();
	~EnemyManager();

	void m_buildWarriorPool();
	void m_buildArcherPool();

	int m_enemyNumber;
	std::list<Warrior*> m_pWarriorPool;
	std::list<Archer*> m_pArcherPool;
	std::vector<Enemy*> m_pEnemyVec;
	static EnemyManager* s_pInstance;
};
