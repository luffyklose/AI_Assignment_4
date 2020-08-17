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

	void generateObstacle();
	void RemoveInvalid();
	void AddStean(DestructibleObstacle* stean);

	void update();
	void draw();
	void Init();
	void exit();
	std::list<DestructibleObstacle*> getDesObsList() { return m_pDestructibleObjectPool; }
	std::vector<DestructibleObstacle*>getSteanVec() { return m_steanVec; }
	
private:
	DestructibleObstacleManager();
	~DestructibleObstacleManager();

	void m_buildDestructibleObjects();

	//int m_desctructileObjectNumber;
	std::list<DestructibleObstacle*> m_pDestructibleObjectPool;
	std::vector<DestructibleObstacle*> m_steanVec;

	static DestructibleObstacleManager* s_pInstance;
};

