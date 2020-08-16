#include "ProjectileManager.h"

ProjectileManager* ProjectileManager::s_pInstance = nullptr;

FireBall* ProjectileManager::generateFireball()
{
	auto fireball = m_pFireBallPool.front();
	fireball->setActive();
	m_pFireBallPool.pop_front();
	m_pFireBallPool.push_back(fireball);

	return fireball;
}

void ProjectileManager::update()
{
	for (auto fireball: m_pFireBallPool)
	{
		if (fireball->IsActive())
		{
			fireball->draw();
		}
	}
}

void ProjectileManager::draw()
{
	for (auto fireball : m_pFireBallPool)
	{
		if (fireball->IsActive())
		{
			fireball->update();
		}
	}
}

ProjectileManager::ProjectileManager():m_FireballNumber(20)
{
	m_buildFireballPool();
}

ProjectileManager::~ProjectileManager()
= default;

void ProjectileManager::m_buildFireballPool()
{
	m_pFireBallPool = std::list<FireBall*>();

	for (int count = 0; count < m_FireballNumber; count++)
	{
		m_pFireBallPool.push_back(new FireBall());
	}
}
