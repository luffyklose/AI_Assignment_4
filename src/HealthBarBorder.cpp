#include "HealthBarBorder.h"

#include "../src/TextureManager.h"

HealthBarBorder::HealthBarBorder(Sprite* sp)
{
	this->m_pSp = sp;

	TextureManager::Instance()->load("../Assets/textures/HealthBarBorder.png", "HealthBarBorder");

	const auto size = TextureManager::Instance()->getTextureSize("HealthBarBorder");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position.x = sp->getTransform()->position.x;
	getTransform()->position.y = sp->getTransform()->position.y - 40;
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	//setType(PLAYERHEALTHBAR);
}

HealthBarBorder::~HealthBarBorder()
= default;

void HealthBarBorder::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("HealthBarBorder", x, y, this->getWidth(), this->getHeight(), 0,255,true);
}

void HealthBarBorder::update()
{
	//const auto size = TextureManager::Instance()->getTextureSize("HealthBarBorder");

	getTransform()->position.x = m_pSp->getTransform()->position.x;
	getTransform()->position.y = m_pSp->getTransform()->position.y - 20;
}

void HealthBarBorder::clean()
{

}
