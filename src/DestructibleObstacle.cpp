#include "DestructibleObstacle.h"

#include "TextureManager.h"

DestructibleObstacle::DestructibleObstacle()
{
	TextureManager::Instance()->load("../Assets/textures/obstacle.png", "obstacle_fullHP");
	TextureManager::Instance()->load("../Assets/textures/obstacle.png", "obstacle_halfHP");
	
	const auto size = TextureManager::Instance()->getTextureSize("obstacle");
	//setWidth(w);
	//setHeight(h);
	

	setType(OBSTACLE);

	reset();
}

DestructibleObstacle::~DestructibleObstacle()
= default;

void DestructibleObstacle::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;


	if (m_curHP > ObstacleMaxHP / 2)
	{
		TextureManager::Instance()->draw("obstacle_fullHP", x, y, getWidth(), getHeight(), 0, 255, true);
	}
	else
	{
		TextureManager::Instance()->draw("obstacle_halfHP", x, y, getWidth(), getHeight(), 0, 255, true);
	}
}

void DestructibleObstacle::update()
{
}

void DestructibleObstacle::clean()
{
}

void DestructibleObstacle::DecHP(int damage)
{
	m_curHealth = m_curHealth - damage;
	if (m_curHealth < 0)
	{
		m_curHealth = 0;
		reset();
	}
}

void DestructibleObstacle::reset()
{
	if (m_bIsActive == true)
	{
		getTransform()->position.x = -1000;
		getTransform()->position.y = -1000;
		m_bIsActive = false;
		m_curHP = ObstacleMaxHP;
	}
}

bool DestructibleObstacle::isActive()
{
	return m_bIsActive;
}

void DestructibleObstacle::setActive()
{
	m_bIsActive;
}
