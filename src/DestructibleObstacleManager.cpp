#include "DestructibleObstacleManager.h"

DestructibleObstacleManager* DestructibleObstacleManager::s_pInstance = nullptr;

DestructibleObstacle* DestructibleObstacleManager::generateObstacle()
{
	auto obstacle = m_pDestructibleObjectPool.front();
	obstacle->setActive();
	m_pDestructibleObjectPool.pop_front();
	m_pDestructibleObjectPool.push_back(obstacle);
	return obstacle;
}


DestructibleObstacleManager::~DestructibleObstacleManager()
= default;

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

DestructibleObstacleManager::DestructibleObstacleManager() :m_desctructileObjectNumber(10)
{
	m_buildDestructibleObjects();
}

void DestructibleObstacleManager::m_buildDestructibleObjects()
{
	m_pDestructibleObjectPool = std::list<DestructibleObstacle*>();

	for (int count = 0; count < m_desctructileObjectNumber; count++)
	{
		m_pDestructibleObjectPool.push_back(new DestructibleObstacle());
	}
}
