#include "HealthBarFiller.h"
#include "../src/TextureManager.h"

HealthBarFiller::HealthBarFiller(Sprite* sp)
{
	this->m_pSp = sp;
	
	TextureManager::Instance()->load("../Assets/textures/HealthBarFiller.png", "HealthBarFiller");

	const auto size = TextureManager::Instance()->getTextureSize("HealthBarFiller");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position.x = sp->getTransform()->position.x;
	getTransform()->position.y = sp->getTransform()->position.y - 40;
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
	TextureManager::Instance()->draw("HealthBarFiller", x, y, this->getWidth(), this->getHeight(), 0,255,true);
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
		this->setWidth((int)(32 * temp));
	}	
	//std::cout << "After: " << this->getWidth() << std::endl;
	
	getTransform()->position.x = m_pSp->getTransform()->position.x;
	getTransform()->position.y = m_pSp->getTransform()->position.y - 20;
}

void HealthBarFiller::clean()
{
	
}
