#include "Explosion.h"
#include "TextureManager.h"

Explosion::Explosion()
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png",
		"spritesheet");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));

	// set frame width
	setWidth(51);

	// set frame height
	setHeight(52);

	getTransform()->position = glm::vec2(200.0f, 200.0f);
	//setType(EXPLOSION);

	m_buildAnimations();

	reset();
}

Explosion::~Explosion()
= default;

void Explosion::draw()
{
	if(getAnimation("explosion").current_frame < getAnimation("explosion").frames.size() - 1)
	{	
		const auto x = getTransform()->position.x;
		const auto y = getTransform()->position.y;

		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("explosion"), x, y, getWidth(), getHeight(), 0.5f, 0, 255, true);
	}
	else
	{		
		reset();
	}
}

void Explosion::update()
{
}

void Explosion::clean()
{
}

void Explosion::reset()
{
	if(m_bIsActive==true)
	{
		getTransform()->position.x = -1000;
		getTransform()->position.y = -1000;
		m_bIsActive = false;
	}
	
}

//bool Explosion::isActive()
//{
//	return m_bIsActive;
//}

void Explosion::setActive()
{
	getAnimation("explosion").current_frame = 0;
	m_bIsActive = true;
}

void Explosion::m_buildAnimations()
{
	Animation explosionAnimation = Animation();

	explosionAnimation.name = "explosion";
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion1"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion2"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion3"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion4"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion5"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion6"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion7"));

	setAnimation(explosionAnimation);
}

int Explosion::getMaxhealth()
{
	return m_curHealth;
}
