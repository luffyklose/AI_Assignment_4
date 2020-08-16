#include "DestructibleObstacleManager.h"

DestructibleObstacleManager* DestructibleObstacleManager::s_pInstance = nullptr;

const int DESOBSTACLENUMBER = 20;


DestructibleObstacleManager::DestructibleObstacleManager()
{
	
}

DestructibleObstacleManager::~DestructibleObstacleManager()
= default;

void DestructibleObstacleManager::Init()
{
	m_buildDestructibleObjects();
}

void DestructibleObstacleManager::generateObstacle()
{
	auto obstacle = m_pDestructibleObjectPool.front();
	obstacle->setActive();
	m_pDestructibleObjectPool.pop_front();
	m_pDestructibleObjectPool.push_back(obstacle);
}

void DestructibleObstacleManager::update()
{
	for (auto obstacle: m_pDestructibleObjectPool)
	{
		if (obstacle->isActive())
		{
			//obstacle->update();
		}
	}
}

void DestructibleObstacleManager::draw()
{
	for (auto obstacle : m_pDestructibleObjectPool)
	{
		if (obstacle->isActive())
		{
			obstacle->draw();
		}
	}
}

void DestructibleObstacleManager::m_buildDestructibleObjects()
{
	m_pDestructibleObjectPool = std::list<DestructibleObstacle*>();

	for (int count = 0; count < DESOBSTACLENUMBER; count++)
	{
		m_pDestructibleObjectPool.push_back(new DestructibleObstacle());
	}
}
