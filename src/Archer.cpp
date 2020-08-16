#include "Archer.h"
#include <algorithm>
#include "TextureManager.h"
#include"Util.h"
#include "SoundManager.h"

Archer::Archer()
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/slime.txt",
		"../Assets/sprites/slime.png",
		"slime");

	/*setSpriteSheet(TextureManager::Instance()->getSpriteSheet("plane"));*/
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("slime"));

	// set frame width
	setWidth(32);

	// set frame height
	setHeight(32);

	//alt pending
	//getTransform()->position = glm::vec2(x,y);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLANE);

	this->m_pFiller = new HealthBarFiller(this);
	this->m_pBorder = new HealthBarBorder(this);

	m_detectionRadius = 200;
	m_accel = 0.2;
	m_velMax = 2.0;

	m_buildAnimations();

	reset();
}

Archer::~Archer()
= default;

void Archer::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	if (m_isPatrol)
	{
		TextureManager::Instance()->playAnimation(
			"slime", getAnimation("run"),
			x, y, getWidth(), getHeight(), 0.5f, 0, 255, true);
	}
	else
	{
		TextureManager::Instance()->playAnimation(
			"slime", getAnimation("idle"),
			x, y, getWidth(), getHeight(), 0.5f, 0, 255, true);
	}


	m_pBorder->draw();
	m_pFiller->draw();
}

void Archer::update()
{
	if (m_isPatrol)
	{
		MovePlanetoPatrolNode();
		SoundManager::Instance().playSound("engine", 0, -1);
	}

	m_pFiller->update();
	m_pBorder->update();
}

void Archer::clean()
{
}

void Archer::reset()
{
	if (m_bIsActive == true)
	{
		getTransform()->position.x = -1000;
		getTransform()->position.y = -1000;
		m_bIsActive = false;
	}
}

void Archer::setActive()
{
	m_bIsActive = true;
}

void Archer::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-idle-3"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-run-3"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-run-4"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-run-5"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-run-6"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("slime-run-7"));

	setAnimation(runAnimation);
}

void Archer::MovePlane()
{
	switch (m_dir)
	{
	case left:
	{
		getRigidBody()->velocity.x += m_accel;
		getRigidBody()->velocity.x = std::min(getRigidBody()->velocity.x, m_velMax);
		getRigidBody()->velocity = glm::vec2(-m_vel, 0.0f);
		break;
	}
	case right:
	{
		getRigidBody()->velocity.x += m_accel;
		getRigidBody()->velocity.x = std::min(getRigidBody()->velocity.x, m_velMax);
		getRigidBody()->velocity = glm::vec2(m_vel, 0.0f);
		break;
	}
	case up:
	{
		getRigidBody()->velocity.y += m_accel;
		getRigidBody()->velocity.y = std::min(getRigidBody()->velocity.y, m_velMax);
		getRigidBody()->velocity = glm::vec2(0.0f, -m_vel);
		break;
	}
	case down:
	{
		getRigidBody()->velocity.y += m_accel;
		getRigidBody()->velocity.y = std::min(getRigidBody()->velocity.y, m_velMax);
		getRigidBody()->velocity = glm::vec2(0.0f, m_vel);
		break;
	}
	default:break;
	}
}

void Archer::SetNextNode()
{
	//std::cout << "Set Node" << m_nodeIndex<<" "<<(int)s_path.size()<<std::endl;
	if (m_nodeIndex < (int)m_path.size() - 1)
	{
		//std::cout << "Before: Move from (" << m_currentNode->x/32 << "," << m_currentNode->y/32 << ") to (" << m_targetNode->x/32 << "," << m_targetNode->y/32 << ")." << std::endl;
		m_currentNode = m_targetNode;
		m_targetNode = m_path[++m_nodeIndex]->GetToNode();
		//std::cout << "After: Move from (" << m_currentNode->x/32 << "," << m_currentNode->y/32<< ") to (" << m_targetNode->x/32 << "," << m_targetNode->y/32 << ")." << std::endl;
	}
	else
	{
		std::cout << "the last one" << std::endl;
		m_currentNode = m_targetNode;
	}
}

std::vector<PathConnection*> Archer::getPath()
{
	return m_path;
}

void Archer::getDir()
{
	if (m_targetNode->getTransform()->position.x == m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y > m_currentNode->getTransform()->position.y)
		m_dir = down;
	if (m_targetNode->getTransform()->position.x == m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y < m_currentNode->getTransform()->position.y)
		m_dir = up;
	if (m_targetNode->getTransform()->position.x < m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y == m_currentNode->getTransform()->position.y)
		m_dir = left;
	if (m_targetNode->getTransform()->position.x > m_currentNode->getTransform()->position.x&& m_targetNode->getTransform()->position.y == m_currentNode->getTransform()->position.y)
		m_dir = right;
	if (m_currentNode == m_targetNode)
	{
		PathNode* temp = start_point;
		start_point = end_point;
		end_point = temp;
		temp = nullptr;
		m_nodeIndex = 0;
	}
}
