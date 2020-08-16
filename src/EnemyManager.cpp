#include "EnemyManager.h"
EnemyManager* EnemyManager::s_pInstance = nullptr;

Enemy* EnemyManager::generateArcher()
{
	auto archer = m_pArcherPool.front();
	archer->setActive();
	m_pArcherPool.pop_front();
	m_pArcherPool.push_back(archer);

	return archer;
}

Enemy* EnemyManager::generateWarrior()
{
	auto warrior = m_pArcherPool.front();
	warrior->setActive();
	m_pArcherPool.pop_front();
	m_pArcherPool.push_back(warrior);

	return warrior;
}

void EnemyManager::update()
{
	for (auto warrior : m_pWarriorPool)
	{
		if (warrior->isActive())
		{
			warrior->update();
		}
	}

	for (auto archer : m_pArcherPool)
	{
		if (archer->isActive())
		{
			archer->update();
		}
	}
} 

void EnemyManager::draw()
{
	for (auto warrior: m_pWarriorPool)
	{
		if (warrior->isActive())
		{
			warrior->draw();
		}
	}

	for (auto archer: m_pArcherPool)
	{
		if (archer->isActive())
		{
			archer->draw();
		}
	}
}

EnemyManager::EnemyManager():m_enemyNumber(5)
{
	m_buildArcherPool();
	m_buildWarriorPool();
}

EnemyManager::~EnemyManager() = default;

void EnemyManager::m_buildWarriorPool()
{
	m_pWarriorPool = std::list<Warrior*>();

	for (int count = 0; count < m_enemyNumber; count++)
	{
		m_pWarriorPool.push_back(new Warrior());
	}
}

void EnemyManager::m_buildArcherPool()
{
	m_pArcherPool = std::list<Archer*>();

	for (int count = 0; count < m_enemyNumber; count++)
	{
		m_pArcherPool.push_back(new Archer());
	}
}

