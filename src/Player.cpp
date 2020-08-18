#include "Player.h"

#include "SoundManager.h"
#include "TextureManager.h"
#include "Util.h"

const int MELEECD = 40;
const int SHOOTCD = 50;
const int PLAYERRANGEDAMAGE = 35;
const int PLAYERMELEEDAMAGE = 20;
const int PLAYERHITRECOVERTIME = 40;

Player::Player(float x,float y): m_currentState(PLAYER_IDLE), m_meleeRange(20)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/hero.txt",
		"../Assets/sprites/hero.png",
		"hero");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("hero"));
	
	// set frame width
	setWidth(40);

	// set frame height
	setHeight(40);

	getTransform()->position = glm::vec2(x,y);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->maxSpeed = 10.0f;
	getRigidBody()->isColliding = false;
	setType(PLAYER);
	
	m_curHealth = PLAYERMAXHEALTH;
	this->m_pFiller = new HealthBarFiller(this);
	this->m_pBorder = new HealthBarBorder(this);

	m_hitRecoverCounter = PLAYERHITRECOVERTIME;
	m_meleeCounter = MELEECD;
	m_shootCounter = SHOOTCD;
	m_dir = right;
	
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
	if (m_currentState == PLAYER_BEHIT)
	{
		switch (m_dir)
		{
		case left:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("be_hit_right"),
				x, y, getWidth(), getHeight(), 0.2f, 0, 255, true, SDL_FLIP_HORIZONTAL);
			std::cout << "be hit left" << std::endl;
			break;
		case right:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("be_hit_right"),
				x, y, getWidth(), getHeight(), 0.2f, 0, 255, true);
			std::cout << "be hit right" << std::endl;
		case up:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("be_hit_up"),
				x, y, getWidth(), getHeight(), 0.2f, 0, 255, true);
			std::cout << "be hit up" << std::endl;
			break;
		case down:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("be_hit_up"),
				x, y, getWidth(), getHeight(), 0.2f, 180, 255, true);
			std::cout << "be hit down" << std::endl;
			break;
		default:break;
		}
	}
	else if (m_currentState == PLAYER_IDLE)
	{
		if (m_dir == right || m_dir == up)
		{
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("idle"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
		}
		else if (m_dir == left || m_dir == down)
		{
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("idle"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		}
	}
	else if (m_currentState == PLAYER_WALK)
	{
		switch (m_dir)
		{
		case left:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("walk_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true, SDL_FLIP_HORIZONTAL);
			break;
		case right:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("walk_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			break;
		case up:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("walk_up"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			break;
		case down:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("walk_down"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			break;
		default:break;
		}
	}
	else if (m_currentState == PLAYER_HIT)
	{
		switch (m_dir)
		{
		case left:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("hit_right"),
				x, y, getWidth(), getHeight(), 0.2f, 0, 255, true, SDL_FLIP_HORIZONTAL);
			if (getAnimation("hit_right").current_frame >= getAnimation("hit_right").frames.size() - 1)
			{
				//this->getTransform()->position += glm::vec2(0.5f * this->getWidth(), 0.0f);
				//this->setWidth(this->getWidth() * 0.5);				
				getAnimation("hit_right").current_frame = 0;
				m_currentState = PLAYER_IDLE;
			}
			break;
		case right:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("hit_right"),
				x, y, getWidth(), getHeight(), 0.2f, 0, 255, true);
			if (getAnimation("hit_right").current_frame >= getAnimation("hit_right").frames.size() - 1)
			{
				getAnimation("hit_right").current_frame = 0;
				m_currentState = PLAYER_IDLE;
			}
			break;
		case up:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("hit_up"),
				x, y, getWidth(), getHeight(), 0.2f, 0, 255, true);
			if (getAnimation("hit_up").current_frame >= getAnimation("hit_up").frames.size() - 1)
			{
				getAnimation("hit_up").current_frame = 0;
				m_currentState = PLAYER_IDLE;
			}
			break;
		case down:
			TextureManager::Instance()->playAnimation(
				"hero", getAnimation("hit_down"),
				x, y, getWidth(), getHeight(), 0.2f, 0, 255, true);
			if (getAnimation("hit_down").current_frame >= getAnimation("hit_down").frames.size() - 1)
			{
				getAnimation("hit_down").current_frame = 0;
				m_currentState = PLAYER_IDLE;
			}
			break;
		default:break;
		}
	}

	m_pBorder->draw();
	m_pFiller->draw();
	
}

void Player::update()
{
	m_pFiller->update();
	m_pBorder->update();
	setCurNode();

	if (m_hitRecoverCounter < PLAYERHITRECOVERTIME)
		m_hitRecoverCounter++;
	else if (m_hitRecoverCounter >= PLAYERHITRECOVERTIME && m_currentState == PLAYER_BEHIT)
		m_currentState = PLAYER_IDLE;
	
	if (m_meleeCounter < MELEECD)
		m_meleeCounter++;
	if (m_shootCounter < SHOOTCD)
		m_shootCounter++;
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

void Player::setPlayerState(PlayerState state)
{
	m_currentState = state;
}

bool Player::canMelee()
{
	return m_meleeCounter >= MELEECD;
}

bool Player::canShoot()
{
	return m_shootCounter >= SHOOTCD;
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
	behit_rightAnimation.name = "be_hit_right";
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_right_0"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_right_1"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_right_2"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_right_3"));
	setAnimation(behit_rightAnimation);

	Animation behit_upAnimation = Animation();
	behit_upAnimation.name = "be_hit_up";
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_up_0"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_up_1"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_up_2"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("hero_behit_up_3"));
	setAnimation(behit_upAnimation);

	Animation behit_downAnimation = Animation();
	behit_upAnimation.name = "be_hit_down";
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
	SoundManager::Instance().playSound("melee", 0, 1);
}
