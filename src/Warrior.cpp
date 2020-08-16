#include "Warrior.h"
#include <algorithm>
#include "TextureManager.h"
#include"Util.h"
#include "SoundManager.h"

Warrior::Warrior()
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

Warrior::~Warrior()
= default;

void Warrior::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the plane sprite with simple propeller animation
	/*TextureManager::Instance()->playAnimation(
		"spritesheet", getAnimation("plane"),
		x+10, y+10, getWidth(), getHeight(), 0.5f, m_angle, 255, true);*/

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

void Warrior::update()
{
	//if (m_path.empty())
	//{
	//	std::cout << "Path Empty" << std::endl;
	//}
	////std::cout << "Running" << std::endl;
	////std::cout << MAMA::Distance((int)(GetDstP()->x + GetDstP()->w / 2.0f), m_targetNode->x+16, (int)(GetDstP()->y + GetDstP()->h / 2.0f), m_targetNode->y+16) << std::endl;
	//if (Util::distance({ (getTransform()->position.x + getWidth() / 2.0f), m_targetNode->getTransform()->position.x + 20 }, { (getTransform()->position.y + getHeight() / 2.0f), (m_targetNode->getTransform()->position.y + 20) }) <= 10.0)
	//	SetNextNode();
	////std::cout << "Current: " << m_currentNode->x /32<< " " << m_currentNode->y/32 << std::endl;
	////std::cout << "Target: " << m_targetNode->x/32 << " " << m_targetNode->y /32<< std::endl;
	//getDir();
	////std::cout << m_dir << std::endl;
	//MovePlane();
	//
	if(m_isPatrol)
	{
		MovePlanetoPatrolNode();
		SoundManager::Instance().playSound("engine", 0, -1);
	}

	m_pFiller->update();
	m_pBorder->update();
	
	//getTransform()->position += getRigidBody()->velocity;
	//getRigidBody()->velocity *= getRigidBody()->velocity * 0.9f;
}

void Warrior::clean()
{
}

void Warrior::m_buildAnimations()
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

void Warrior::MovePlane()
{
	// Finish up movement.
	// Now use the new slight rotation to generate dx and dy as normal.
	//std::cout <<"before "<< m_vel << std::endl;
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
	//std::cout << "after " << m_dX<<" "<<m_dY << std::endl;
}

void Warrior::SetNextNode()
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

/*void Warrior::setPath(std::vector<PathConnection*> path)
{
	m_path = path;
	m_currentNode = m_path[0]->GetFromNode();
	m_targetNode = m_path[0]->GetToNode();
}*/

std::vector<PathConnection*> Warrior::getPath()
{
	return m_path;
}


void Warrior::getDir()
{
	if (m_targetNode->getTransform()->position.x == m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y > m_currentNode-> getTransform()->position.y)
		m_dir = down;
	if (m_targetNode->getTransform()->position.x == m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y < m_currentNode->getTransform()->position.y)
		m_dir = up;
	if (m_targetNode->getTransform()->position.x < m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y == m_currentNode->getTransform()->position.y)
		m_dir = left;
	if (m_targetNode->getTransform()->position.x > m_currentNode->getTransform()->position.x && m_targetNode-> getTransform()->position.y == m_currentNode->getTransform()->position.y)
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

void Warrior::reset()
{
	if (m_bIsActive == true)
	{
		getTransform()->position.x = -1000;
		getTransform()->position.y = -1000;
		m_bIsActive = false;
	}
}

void Warrior::setActive()
{
	//set animation
	
	m_bIsActive = true;
	
}

