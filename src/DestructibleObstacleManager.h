#pragma once
#include "DestructibleObstacle.h"

class DestructibleObstacleManager
{
public:
	static DestructibleObstacleManager* Instance()
	{
		if (s_pInstance == nullptr)
			s_pInstance = new DestructibleObstacleManager();
		return s_pInstance;
	}

	DestructibleObstacle* generateObstacle();

	void update();
	void draw();
	
private:
	DestructibleObstacleManager();
	~DestructibleObstacleManager();

	void m_buildDestructibleObjects();

	int m_desctructileObjectNumber;
	std::list<DestructibleObstacle*> m_pDestructibleObjectPool;

	static DestructibleObstacleManager* s_pInstance;
};

