#include "ProjectileManager.h"
#include <iostream>


ProjectileManager* ProjectileManager::s_pInstance = nullptr;
//std::list<FireBall*> ProjectileManager::m_pFireBallPool;

const int FIREBALNUMBER = 40;

ProjectileManager::ProjectileManager()
{
	
}

ProjectileManager::~ProjectileManager()
= default;

void ProjectileManager::Init()
{
	m_buildFireballPool();
}

void ProjectileManager::RemoveInvalid()
{

	for (auto fireball = m_playerFireVec.begin(); fireball != m_playerFireVec.end();)
	{
		if (! (*fireball)->isActive())
		{
			fireball = m_playerFireVec.erase(fireball);
		}
		else
		{
			++fireball;
		}
	}

	for (auto fireball = m_enemyFireVec.begin(); fireball != m_enemyFireVec.end();)
	{
		if (!(*fireball)->isActive())
		{
			fireball = m_enemyFireVec.erase(fireball);
		}
		else
		{
			++fireball;
		}
	}
	
	/*for (auto fireball : m_enemyFireVec)
	{
		if (fireball->isActive() == false)
		{
			fireball = nullptr;
		}
	}
	if (!m_enemyFireVec.empty())
	{
		m_enemyFireVec.erase(remove(m_enemyFireVec.begin(), m_enemyFireVec.end(), nullptr), m_enemyFireVec.end());
	}*/
}

void ProjectileManager::generateFireball()
{
	auto fireball = m_pFireBallPool.front();
	fireball->setActive();
	m_pFireBallPool.pop_front();
	m_pFireBallPool.push_back(fireball);
}

void ProjectileManager::update()
{
	for (auto fireball : m_pFireBallPool)
	{
		if (fireball->isActive())
		{
			fireball->update();
		}
	}
}

void ProjectileManager::draw()
{
	for (auto fireball: m_pFireBallPool)
	{
		if (fireball->isActive())
		{
			fireball->draw();
		}
	}
}

void ProjectileManager::m_buildFireballPool()
{
	m_pFireBallPool = std::list<FireBall*>();

	for (int count = 0; count < FIREBALNUMBER; count++)
	{
		m_pFireBallPool.push_back(new FireBall());
	}
}

void ProjectileManager::exit()
{
	for(auto fireball:m_pFireBallPool)
	{
		delete fireball;
		fireball = nullptr;
	}
	if(!m_playerFireVec.empty())
	{
		for (auto fireball : m_playerFireVec)
		{
			fireball = nullptr;
		}
		m_playerFireVec.erase(remove(m_playerFireVec.begin(), m_playerFireVec.end(), nullptr), m_playerFireVec.end());
	}
	if (!m_enemyFireVec.empty())
	{
		for (auto fireball : m_enemyFireVec)
		{
			fireball = nullptr;
		}
		m_enemyFireVec.erase(remove(m_enemyFireVec.begin(), m_enemyFireVec.end(), nullptr), m_enemyFireVec.end());
	}
	m_pFireBallPool.clear();

	m_playerFireVec.clear();
	m_playerFireVec.shrink_to_fit();

	m_enemyFireVec.clear();
	m_enemyFireVec.shrink_to_fit();
}

void ProjectileManager::addFireBall2EnemyVec(FireBall* fireball)
{
	m_enemyFireVec.push_back(fireball);
}

void ProjectileManager::addFireBall2PlayerVec(FireBall* fireball)
{
	m_playerFireVec.push_back(fireball);
}
