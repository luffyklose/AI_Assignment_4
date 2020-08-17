#include "DestructibleObstacleManager.h"

#include <iostream>

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
	//std::cout << "stean number:" << m_pDestructibleObjectPool.size() << std::endl;
}

void DestructibleObstacleManager::exit()
{
	for (auto stean : m_pDestructibleObjectPool)
	{
		delete stean;
		stean = nullptr;
	}

	for (auto stean : m_steanVec)
	{
		delete stean;
		stean = nullptr;
	}

	m_pDestructibleObjectPool.clear();
	m_steanVec.clear();
	m_steanVec.shrink_to_fit();
}

void DestructibleObstacleManager::generateObstacle()
{
	auto obstacle = m_pDestructibleObjectPool.front();
	obstacle->setActive();
	m_pDestructibleObjectPool.pop_front();
	m_pDestructibleObjectPool.push_back(obstacle);
}

void DestructibleObstacleManager::AddStean(DestructibleObstacle* stean)
{
	m_steanVec.push_back(stean);
}


void DestructibleObstacleManager::RemoveInvalid()
{
	for (auto stean = m_steanVec.begin(); stean != m_steanVec.end();)
	{
		if (!(*stean)->isActive())
		{
			stean = m_steanVec.erase(stean);
		}
		else
		{
			++stean;
		}
	}
}

void DestructibleObstacleManager::update()
{
	for (auto obstacle: m_pDestructibleObjectPool)
	{
		if (obstacle->isActive())
		{
			obstacle->update();
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
