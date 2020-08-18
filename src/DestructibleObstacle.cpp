#include "DestructibleObstacle.h"
#include "TextureManager.h"
#include"ExplosionManager.h"

DestructibleObstacle::DestructibleObstacle()
{
	TextureManager::Instance()->load("../Assets/textures/stean_full.png", "fullStean");
	TextureManager::Instance()->load("../Assets/textures/stean_broken.png", "brokenStean");
	
	const auto size = TextureManager::Instance()->getTextureSize("fullStean");
	setWidth(40);
	setHeight(40);	

	setType(OBSTACLE);
	m_curHealth = STEANMAXHP;
	this->m_pFiller = new HealthBarFiller(this);
	this->m_pBorder = new HealthBarBorder(this);
	
	reset();
}

DestructibleObstacle::~DestructibleObstacle()
= default;

void DestructibleObstacle::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;


	if (m_curHealth > STEANMAXHP / 2)
	{
		TextureManager::Instance()->draw("fullStean", x, y, getWidth(), getHeight(), 0, 255, true);
	}
	else
	{
		TextureManager::Instance()->draw("brokenStean", x, y, getWidth(), getHeight(), 0, 255, true);
	}
	
	m_pBorder->draw();
	m_pFiller->draw();
}

void DestructibleObstacle::update()
{
	m_pFiller->update();
	m_pBorder->update();
}

void DestructibleObstacle::clean()
{
}

void DestructibleObstacle::DecHP(int damage)
{
	m_curHealth = m_curHealth - damage;
	if (m_curHealth <= 0)
	{
		m_curHealth = 0;		
		ExplosionManager::Instance()->generateExplosion();
		auto explosion = ExplosionManager::Instance()->getExplosionList().back();
		explosion->getTransform()->position = this->getTransform()->position;
		reset();
	}
}

void DestructibleObstacle::reset()
{
	getTransform()->position.x = -1000;
	getTransform()->position.y = -1000;
	m_bIsActive = false;
	m_row = -100;
	m_col = -100;
}

//bool DestructibleObstacle::isActive()
//{
//	return m_bIsActive;
//}

void DestructibleObstacle::setActive()
{
	m_bIsActive=true;
	m_curHealth = STEANMAXHP;
}
