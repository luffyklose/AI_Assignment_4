#include "Fireball.h"
#include "ExplosionManager.h"
#include "TextureManager.h"

const float FLYSPEED = 4.0f;
const int FIREBALLDAMAGE = 20;

FireBall::FireBall() :m_face(1),m_damage(FIREBALLDAMAGE),m_speed(FLYSPEED)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/fireball.txt",
		"../Assets/sprites/fireball.png",
		"fireball");
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("fireball"));

	// set frame width
	setWidth(16);

	// set frame height
	setHeight(16);

	setType(FIREBALL);

	//getTransform()->position = glm::vec2(x, y);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	//getRigidBody()->maxSpeed = FLYSPEED;
	getRigidBody()->isColliding = false;

	m_buildAnimations();

	reset();
}

FireBall::~FireBall()
= default;

void FireBall::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	switch (m_dir)
	{
	case right:
		TextureManager::Instance()->playAnimation(
			"fireball", getAnimation("fire"),
			x, y, getWidth(), getHeight(), 0.5f, 0, 255, true);
		break;
	case left:
		TextureManager::Instance()->playAnimation(
			"fireball", getAnimation("fire"),
			x, y, getWidth(), getHeight(), 0.5f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case up:
		TextureManager::Instance()->playAnimation(
			"fireball", getAnimation("fire"),
			x, y, getWidth(), getHeight(), 0.5f, 90, 255, true);
		break;
	case down:
		TextureManager::Instance()->playAnimation(
			"fireball", getAnimation("fire"),
			x, y, getWidth(), getHeight(), 0.5f, -90, 255, true);
		break;
	default:
		break;
	}
}

void FireBall::update()
{
	/*switch (m_dir)
	{
	case right:
		this->getTransform()->position.x += (float)m_speed * 1;
		break;
	case left:
		this->getTransform()->position.x += (float)m_speed * (-1);
		break;
	case up:
		this->getTransform()->position.y += (float)m_speed * (-1);
		break;
	case down:
		this->getTransform()->position.y += (float)m_speed * 1;
		break;
	}*/
	
	//this->getTransform()->position.x += m_speed * m_face;
	this->getTransform()->position += this->getRigidBody()->velocity;
	if (this->getTransform()->position.x < 0 || this->getTransform()->position.x > Config::SCREEN_WIDTH 
		|| this->getTransform()->position.y < 0 || this->getTransform()->position.y > Config::SCREEN_HEIGHT)
		reset();
}

void FireBall::clean()
{

}

void FireBall::reset()
{
	this->getAnimation("fire").current_frame = 0;
	
	if (m_bIsActive == true)
	{
		getTransform()->position.x = -1000;
		getTransform()->position.y = -1000;
		m_bIsActive = false;
	}
}

void FireBall::setActive()
{
	getAnimation("fireball").current_frame = 0;
	m_bIsActive = true;
}

void FireBall::m_buildAnimations()
{
	Animation fireAnimation = Animation();

	fireAnimation.name = "fire";
	fireAnimation.frames.push_back(getSpriteSheet()->getFrame("fireball-fire-0"));
	fireAnimation.frames.push_back(getSpriteSheet()->getFrame("fireball-fire-1"));
	fireAnimation.frames.push_back(getSpriteSheet()->getFrame("fireball-fire-2"));
	fireAnimation.frames.push_back(getSpriteSheet()->getFrame("fireball-fire-3"));
	fireAnimation.frames.push_back(getSpriteSheet()->getFrame("fireball-fire-4"));
	fireAnimation.frames.push_back(getSpriteSheet()->getFrame("fireball-fire-5"));

	setAnimation(fireAnimation);
}

void FireBall::setFace(int face)
{
	m_face = face;
}

void FireBall::setIsActive(bool active)
{
	m_bIsActive = active;
	if(active=false)
	{
		ExplosionManager::Instance()->generateExplosion();
		auto explosion = ExplosionManager::Instance()->getExplosionList().back();
		explosion->getTransform()->position = this->getTransform()->position;
	}
}
