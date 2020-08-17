#include "Player.h"
#include "TextureManager.h"
#include "Util.h"

Player::Player(float x,float y): m_currentAnimationState(PLAYER_IDLE_RIGHT)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/hero.txt",
		"../Assets/sprites/hero.png",
		"hero");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("hero"));
	
	// set frame width
	setWidth(48);

	// set frame height
	setHeight(48);

	getTransform()->position = glm::vec2(x,y);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->maxSpeed = 10.0f;
	getRigidBody()->isColliding = false;
	setType(PLAYER);
	
	m_curHealth = PLAYERMAXHEALTH;
	this->m_pFiller = new HealthBarFiller(this);
	this->m_pBorder = new HealthBarBorder(this);
	
	m_buildAnimations();
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case PLAYER_IDLE_RIGHT:
		TextureManager::Instance()->playAnimation("hero", getAnimation("idle"),
			x, y, getWidth(),getHeight(),0.1f, 0, 255, true);
		break;
	case PLAYER_IDLE_LEFT:
		TextureManager::Instance()->playAnimation("hero", getAnimation("idle"),
			x, y, getWidth(), getHeight(),0.1f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_WALK_RIGHT:
		TextureManager::Instance()->playAnimation("hero", getAnimation("walk_right"),
			x, y, getWidth(), getHeight(),0.1f, 0, 255, true);
		break;
	case PLAYER_WALK_LEFT:
		TextureManager::Instance()->playAnimation("hero", getAnimation("walk_right"),
			x, y, getWidth(), getHeight(), 0.1f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_WALK_UP:
		TextureManager::Instance()->playAnimation("hero", getAnimation("walk_up"),
			x, y, getWidth(), getWidth(), 0.1f, 0, 255, true);
	case PLAYER_WALK_DOWN:
		TextureManager::Instance()->playAnimation("hero", getAnimation("walk_down"),
			x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
	default:
		break;
	}

	m_pBorder->draw();
	m_pFiller->draw();
	
}

void Player::update()
{
	m_pFiller->update();
	m_pBorder->update();
	setCurNode();
}

void Player::clean()
{
}

void Player::setCurNode()
{
	float tempDistance = 0.0f;
	for (auto pathnode : NDMA::getPathNodeVec())
	{
		float tempDis = Util::distance(this->getTransform()->position, pathnode->getTransform()->position);;
		if (tempDistance == 0.0f)
		{
			tempDistance = tempDis;
			m_curNode = pathnode;
		}
		else if (tempDis < tempDistance)
		{
			tempDistance = tempDis;
			m_curNode = pathnode;
		}
	}
}

void Player::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::m_buildAnimations()
{
	Animation idleAnimation = Animation();
	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_idle_0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_idle_1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_idle_2"));
	setAnimation(idleAnimation);

	Animation walk_rightAnimation = Animation();
	walk_rightAnimation.name = "walk_right";
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_right_0"));
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_right_1"));
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_right_2"));
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_right_3"));
	setAnimation(walk_rightAnimation);

	Animation walk_upAnimation = Animation();
	walk_upAnimation.name = "walk_up";
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_up_0"));
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_up_1"));
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_up_2"));
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_up_3"));
	setAnimation(walk_upAnimation);

	Animation walk_downAnimation = Animation();
	walk_downAnimation.name = "walk_down";
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_down_0"));
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_down_1"));
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_down_2"));
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_walk_down_3"));
	setAnimation(walk_downAnimation);

	Animation hit_rightAnimation = Animation();
	hit_rightAnimation.name = "hit_right";
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_right_0"));
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_right_1"));
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_right_2"));
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_right_3"));
	setAnimation(hit_rightAnimation);

	Animation hit_upAnimation = Animation();
	hit_upAnimation.name = "hit_up";
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_up_0"));
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_up_1"));
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_up_2"));
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_up_3"));
	setAnimation(hit_upAnimation);

	Animation hit_downAnimation = Animation();
	hit_downAnimation.name = "hit_down";
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_down_0"));
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_down_1"));
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_down_2"));
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_hit_down_3"));
	setAnimation(hit_downAnimation);

	Animation behit_rightAnimation = Animation();
	behit_rightAnimation.name = "behit_right";
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_right_0"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_right_1"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_right_2"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_right_3"));
	setAnimation(behit_rightAnimation);

	Animation behit_upAnimation = Animation();
	behit_upAnimation.name = "behit_up";
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_up_0"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_up_1"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_up_2"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_up_3"));
	setAnimation(behit_upAnimation);

	Animation behit_downAnimation = Animation();
	behit_upAnimation.name = "behit_down";
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_down_0"));
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_down_1"));
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_down_2"));
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_down_3"));
	setAnimation(behit_downAnimation);
}

int Player::getMeleeDamage()
{
	return PLAYERMELEEDAMAGE;
}

int Player::getRangeDamage()
{
	return PLAYERRANGEDAMAGE;
}


void Player::DecHP(int damage)
{
	m_curHealth -= damage;
}
