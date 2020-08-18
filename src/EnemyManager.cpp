#include "EnemyManager.h"
#include "NodeManager.h"

EnemyManager* EnemyManager::s_pInstance = nullptr;
//std::vector<Enemy*> EnemyManager::m_pEnemyVec;
//std::list<Warrior*> EnemyManager::m_pWarriorPool;
//::list<Archer*> EnemyManager::m_pArcherPool;
//Player* EnemyManager::m_target;

const int ENEMYNUMBER = 5;

EnemyManager::EnemyManager()
{
	
}

EnemyManager::~EnemyManager() = default;

void EnemyManager::Init()
{
	m_buildArcherPool();
	m_buildWarriorPool();
}

void EnemyManager::generateArcher(int& enemyNum)
{
	auto archer = m_pArcherPool.front();
	archer->setActive();
	m_pArcherPool.pop_front();
	m_pArcherPool.push_back(archer);
	m_pEnemyVec.push_back(archer);

	/*archer->AddKeyNode(NDMA::getPathNodeVec()[50]);
	archer->AddKeyNode(NDMA::getPathNodeVec()[70]);
	archer->AddKeyNode(NDMA::getPathNodeVec()[130]);
	archer->AddKeyNode(NDMA::getPathNodeVec()[180]);*/
	archer->AddKeyNode(NDMA::getPathNodeVec()[19]);
	archer->AddKeyNode(NDMA::getPathNodeVec()[18]);
	archer->AddKeyNode(NDMA::getPathNodeVec()[20]);
	archer->AddKeyNode(NDMA::getPathNodeVec()[79]);
	archer->AddKeyNode(NDMA::getPathNodeVec()[60]);

	archer->getTransform()->position = archer->getKeyNode()[0]->m_keyNode->getTransform()->position;
	//archer->getRigidBody()->maxSpeed = 2.0f;
	archer->setCurTargetKdyNode(archer->getKeyNode()[1]);

	enemyNum++;
}

void EnemyManager::generateWarrior(int& enemyNum)
{
	auto warrior = m_pWarriorPool.front();
	warrior->setActive();
	m_pWarriorPool.pop_front();
	m_pWarriorPool.push_back(warrior);
	m_pEnemyVec.push_back(warrior);

	warrior->AddKeyNode(NDMA::getPathNodeVec()[235]);
	warrior->AddKeyNode(NDMA::getPathNodeVec()[236]);
	warrior->AddKeyNode(NDMA::getPathNodeVec()[70]);
	warrior->AddKeyNode(NDMA::getPathNodeVec()[180]);
	warrior->AddKeyNode(NDMA::getPathNodeVec()[39]);

	warrior->getTransform()->position = warrior->getKeyNode()[0]->m_keyNode->getTransform()->position;
	//warrior->getRigidBody()->maxSpeed = 2.0f;
	warrior->setCurTargetKdyNode(warrior->getKeyNode()[1]);

	enemyNum++;
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

void EnemyManager::exit()
{
	for(auto enemy:m_pArcherPool)
	{
		delete enemy;
		enemy = nullptr;
	}
	for (auto enemy : m_pWarriorPool)
	{
		delete enemy;
		enemy = nullptr;
	}
	if(!m_pEnemyVec.empty())
	{
		for (auto enemy : m_pEnemyVec)
		{
			enemy = nullptr;
		}
		m_pEnemyVec.erase(remove(m_pEnemyVec.begin(), m_pEnemyVec.end(), nullptr), m_pEnemyVec.end());
	}
	

	m_pArcherPool.clear();
	m_pWarriorPool.clear();
	m_pEnemyVec.clear();
	m_pEnemyVec.shrink_to_fit();
}

void EnemyManager::RemoveInvalid()
{
	for (auto enemy = m_pEnemyVec.begin(); enemy != m_pEnemyVec.end();)
	{
		if (!(*enemy)->isActive())
		{
			enemy = m_pEnemyVec.erase(enemy);
		}
		else
		{
			++enemy;
		}
	}
}

void EnemyManager::addEnemy(Enemy* enemy)
{
	m_pEnemyVec.push_back(enemy);
}

void EnemyManager::m_buildWarriorPool()
{
	m_pWarriorPool = std::list<Warrior*>();

	for (int count = 0; count < ENEMYNUMBER; count++)
	{
		m_pWarriorPool.push_back(new Warrior(m_target));
	}
}

void EnemyManager::m_buildArcherPool()
{
	m_pArcherPool = std::list<Archer*>();

	for (int count = 0; count < ENEMYNUMBER; count++)
	{
		m_pArcherPool.push_back(new Archer(m_target));
	}
}

