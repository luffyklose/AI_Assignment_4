#include "ProjectileManager.h"

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
		if (fireball->IsActive())
		{
			fireball->update();
		}
	}
}

void ProjectileManager::draw()
{
	for (auto fireball: m_pFireBallPool)
	{
		if (fireball->IsActive())
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
