#include "ExplosionManager.h"

ExplosionManager* ExplosionManager::s_pInstance = nullptr;

const int EXPLOSIONNUMBER = 20;

ExplosionManager::ExplosionManager()
{
	m_buildExplosionPool();
}

ExplosionManager::~ExplosionManager()
= default;

void ExplosionManager::generateExplosion()
{
	auto explosion = m_pExplosionPool.front();
	explosion->setActive();
	m_pExplosionPool.pop_front();
	m_pExplosionPool.push_back(explosion);

}

void ExplosionManager::update()
{
	for (auto explosion: m_pExplosionPool)
	{
		if (explosion->isActive())
		{
			explosion->update();
		}
	}
}

void ExplosionManager::draw()
{
	for (auto explosion : m_pExplosionPool)
	{
		if (explosion->isActive())
		{
			explosion->draw();
		}
	}
}

void ExplosionManager::Init()
{
	m_buildExplosionPool();
}

void ExplosionManager::exit()
{
	for (auto explosion : m_pExplosionPool)
	{
		delete explosion;
		explosion = nullptr;
	}
	m_pExplosionPool.clear();
}

void ExplosionManager::RemoveInvalid()
{
	for (auto explosion = m_pExplosionPool.begin(); explosion != m_pExplosionPool.end();)
	{
		if (!(*explosion)->isActive())
		{
			explosion = m_pExplosionPool.erase(explosion);
		}
		else
		{
			++explosion;
		}
	}
}



void ExplosionManager::m_buildExplosionPool()
{
	//create the pool structure
	m_pExplosionPool = std::list<Explosion*>();

	for (int count = 0; count < EXPLOSIONNUMBER;count++)
	{
		m_pExplosionPool.push_back(new Explosion());
	}
}
