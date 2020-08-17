#pragma once
#include "Archer.h"
#include "Enemy.h"
#include "Warrior.h"
#include "Player.h"

class EnemyManager
{
public:
	static EnemyManager* Instance()
	{
		if (s_pInstance == nullptr)
			s_pInstance = new EnemyManager();
		return s_pInstance;
	}
	void generateArcher();
	void generateWarrior();
	void Init();
	void update();
	void draw();
	void exit();
	void RemoveInvalid();
	void addEnemy(Enemy* enemy);

	//setter
	void setTarget(Player* target = nullptr) { m_target = target; }

	//getter
	std::vector<Enemy*> getEnemyVec() { return m_pEnemyVec; }
	Player* getTarget() { return m_target; }
	std::list<Warrior*> getWarriorList() { return m_pWarriorPool; }
	std::list<Archer*> getArcherList() { return m_pArcherPool; }
private:
	EnemyManager();
	~EnemyManager();

	void m_buildWarriorPool();
	void m_buildArcherPool();
public:

private:
	std::list<Warrior*> m_pWarriorPool;
	std::list<Archer*> m_pArcherPool;
	std::vector<Enemy*> m_pEnemyVec;
	Player* m_target;
	
	static EnemyManager* s_pInstance;
};
