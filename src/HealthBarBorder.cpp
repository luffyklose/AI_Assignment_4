#include "HealthBarBorder.h"
#include "../src/TextureManager.h"

const int BORDERWIDTH = 40;
const int BORDERHEIGHT = 5;

HealthBarBorder::HealthBarBorder(Sprite* sp)
{
	this->m_pSp = sp;

	TextureManager::Instance()->load("../Assets/textures/HealthBarBorder.png", "HealthBarBorder");

	const auto size = TextureManager::Instance()->getTextureSize("HealthBarBorder");
	setWidth(BORDERWIDTH);
	setHeight(BORDERHEIGHT);
	getTransform()->position.x = sp->getTransform()->position.x - 0.5f * sp->getWidth();
	getTransform()->position.y = sp->getTransform()->position.y - 30;
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
	TextureManager::Instance()->draw("HealthBarBorder", x, y, this->getWidth(), this->getHeight(), 0,255,false);
}

void HealthBarBorder::update()
{
	//const auto size = TextureManager::Instance()->getTextureSize("HealthBarBorder");

	getTransform()->position.x = m_pSp->getTransform()->position.x - 0.5f * m_pSp->getWidth();
	getTransform()->position.y = m_pSp->getTransform()->position.y - 30;
}

void HealthBarBorder::clean()
{

}
