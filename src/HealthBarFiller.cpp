#include "HealthBarFiller.h"
#include "../src/TextureManager.h"

const int FILLERWIDTH = 40;
const int FILLERHEIGHT = 5;

HealthBarFiller::HealthBarFiller(Sprite* sp)
{
	this->m_pSp = sp;
	
	TextureManager::Instance()->load("../Assets/textures/HealthBarFiller.png", "HealthBarFiller");

	const auto size = TextureManager::Instance()->getTextureSize("HealthBarFiller");
	setWidth(FILLERWIDTH);
	setHeight(FILLERHEIGHT);
	getTransform()->position.x = sp->getTransform()->position.x - 0.5f * sp->getWidth();
	getTransform()->position.y = sp->getTransform()->position.y - 30;
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	//setType(PLAYERHEALTHBAR);
}

HealthBarFiller::~HealthBarFiller()
= default;

void HealthBarFiller::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("HealthBarFiller", x, y, this->getWidth(), this->getHeight(), 0,255,false);
}

void HealthBarFiller::update()
{
	const auto size = TextureManager::Instance()->getTextureSize("PlayerHealthBarFiller");

	//std::cout << "health: " << m_pSp->getCurHealth() << " " << m_pSp->getMaxhealth() << std::endl;
	float temp = (float)m_pSp->getCurHealth() / (float)m_pSp->getMaxhealth();
	//std::cout << "per: " << temp << std::endl;
	//std::cout << "Before: " << this->getWidth() << std::endl;
	if(temp>=0)
	{
		this->setWidth((int)(FILLERWIDTH * temp));
	}	
	//std::cout << "After: " << this->getWidth() << std::endl;
	
	getTransform()->position.x = m_pSp->getTransform()->position.x - 0.5f * m_pSp->getWidth();
	getTransform()->position.y = m_pSp->getTransform()->position.y - 30;
}

void HealthBarFiller::clean()
{
	
}
