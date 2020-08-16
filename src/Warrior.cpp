#include "Warrior.h"
#include <algorithm>
#include "CollisionManager.h"
#include "TextureManager.h"
#include"Util.h"
#include "SoundManager.h"

const int MELEERANGE = 20;
const int MELEEDAMAGE = 20;
const int MELEECD = 100;
const int DETECTRANGE = 200;

Warrior::Warrior(Player* player):Enemy(player)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/slime.txt",
		"../Assets/sprites/slime.png",
		"slime");
	
	/*setSpriteSheet(TextureManager::Instance()->getSpriteSheet("plane"));*/
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("slime"));

	// set frame width
	setWidth(40);
	
	// set frame height
	setHeight(40);

	//alt pending
	//getTransform()->position = glm::vec2(x,y);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(WARRIOR);

	this->m_pFiller = new HealthBarFiller(this);
	this->m_pBorder = new HealthBarBorder(this);
	
	m_detectionRadius = DETECTRANGE;
	m_accel = 0.2;
	m_velMax = 2.0;

	m_outerState = FIGHT;
	m_innerState = PATROL;

	m_withinMeleeRange = false;
	m_attackMode = false;

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
	//std::cout << "distance: " << Util::distance(this->getTransform()->position, m_pTargetPlayer->getTransform()->position) << std::endl;
	if (Util::distance(this->getTransform()->position, m_pTargetPlayer->getTransform()->position) < 60.0f)
		m_withinMeleeRange = true;
	else
		m_withinMeleeRange = false;

	m_checkCurrentConditions();
	//std::cout << m_outerState << " " << m_innerState << std::endl;
	m_stateMachineUpdate();

	m_meleeCounter++;

	//if(m_isPatrol)
	//{
	//	//MovePlanetoPatrolNode();
	//	//PatrolMove();
	//	//Move2LOS();
	//	//setAttackNode();
	//	//Move2NearestAttackNode();
	//	//Flee();
	//	SoundManager::Instance().playSound("engine", 0, -1);
	//}

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

void Warrior::MoveWarrior()
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

//void Warrior::SetNextNode()
//{
//	//std::cout << "Set Node" << m_nodeIndex<<" "<<(int)s_path.size()<<std::endl;
//	if (m_nodeIndex < (int)m_path.size() - 1)
//	{
//		//std::cout << "Before: Move from (" << m_currentNode->x/32 << "," << m_currentNode->y/32 << ") to (" << m_targetNode->x/32 << "," << m_targetNode->y/32 << ")." << std::endl;
//		m_currentNode = m_targetNode;
//		m_targetNode = m_path[++m_nodeIndex]->GetToNode();
//		//std::cout << "After: Move from (" << m_currentNode->x/32 << "," << m_currentNode->y/32<< ") to (" << m_targetNode->x/32 << "," << m_targetNode->y/32 << ")." << std::endl;
//	}
//	else
//	{
//		std::cout << "the last one" << std::endl;
//		m_currentNode = m_targetNode;
//	}
//}

void Warrior::m_checkCurrentConditions()
{
	if (m_curHealth >= 25)
	{
		if (m_attackMode)
		{
			if (!m_hasLOS)
			{
				// check the distance
				std::cout << "In Melee Range: " << m_withinMeleeRange << std::endl;
				if (m_withinMeleeRange)
				{
					m_innerState = MELEE_ATTACK;
					//std::cout << "change 2 melee" << std::endl;
				}
				else
				{
					m_innerState = MOVE_TO_MELEE;
					//std::cout << "change 2 move 2 melee" << std::endl;
				}
			}
			else
			{
				m_innerState = MOVE_TO_LOS;
				//std::cout << "change 2 move 2 los" << std::endl;
			}
		}
		else if (m_DetectPlayer)
		{
			if (!m_hasLOS)
			{
				// check the distance
				std::cout << m_withinMeleeRange << std::endl;
				m_attackMode = true;
				if (m_withinMeleeRange)
				{
					m_innerState = MELEE_ATTACK;
					//std::cout << "change 2 melee" << std::endl;
				}
				else
				{
					m_innerState = MOVE_TO_MELEE;
					//std::cout << "change 2 move 2 melee" << std::endl;
				}
			}
			else
			{
				m_innerState = MOVE_TO_LOS;
				//std::cout << "change 2 move 2 los" << std::endl;
			}
		}
	}
	else
	{
		m_outerState = FLIGHT;
		//std::cout << "change 2 flee" << std::endl;
	}
}

void Warrior::m_stateMachineUpdate()
{
	switch (m_outerState)
	{
	case FIGHT:
		switch (m_innerState)
		{
		case PATROL:
		{
			// Patrol Action
			PatrolMove();
			//std::cout << "Patroling..." << std::endl;
			break;
		}
		case MELEE_ATTACK:
		{
			// Perform Melee Attack Action
			Melee();
			std::cout << "Meleeing..." << std::endl;
			break;
		}
		case MOVE_TO_LOS:
		{
			// Move 2 LOS Action
			Move2LOS();
			//std::cout << "Moving to LOS..." << std::endl;
			break;
		}
		case MOVE_TO_MELEE:
		{
			// Move 2 Melee Range Action
			setAttackNode();
			Move2NearestAttackNode();
			//std::cout << "Moving to Attack..." << std::endl;
			break;
		}
		}
		break;
	case FLIGHT:
	{
		// Flee Action
		Flee();
		std::cout << "Fleeing..." << std::endl;
		break;
	}
	}
}

void Warrior::setAttackNode()
{
	if (m_pTargetPlayer == nullptr)
		return;
	PathNode* PlayerCurNode = nullptr;
	float playerDistance = 0.0f;
	m_NodeCanAttack.clear();
	m_NodeCanAttack.shrink_to_fit();
	for (auto pathnode : NDMA::getPathNodeVec())
	{
		float tempDistance = Util::distance(m_pTargetPlayer->getTransform()->position, pathnode->getTransform()->position);
		if (playerDistance == 0.0f)
		{
			playerDistance = tempDistance;
			PlayerCurNode = pathnode;
		}
		else if (tempDistance < playerDistance)
		{
			playerDistance = tempDistance;
			PlayerCurNode = pathnode;
		}
	}
	if (PlayerCurNode->GetConnections().empty())
		return;
	for (auto pathconnection : PlayerCurNode->GetConnections())
	{
		m_NodeCanAttack.push_back(pathconnection->GetToNode());
	}
	//std::cout << "Node number: " << (int)m_NodeCanAttack.size() << std::endl;
}

void Warrior::Move2NearestAttackNode()
{
	if (m_NodeCanAttack.empty())
		return;
	PathNode* AttackNode = nullptr;
	float tempNearestDis = 0.0f;
	for (auto pathnode : m_NodeCanAttack)
	{
		if (tempNearestDis == 0.0f)
		{
			tempNearestDis = Util::distance(this->getTransform()->position, pathnode->getTransform()->position);
			AttackNode = pathnode;
		}
		else if (Util::distance(this->getTransform()->position, pathnode->getTransform()->position) < tempNearestDis)
		{
			tempNearestDis = Util::distance(this->getTransform()->position, pathnode->getTransform()->position);
			AttackNode = pathnode;
		}
	}
	if (Util::distance(this->getTransform()->position, AttackNode->getTransform()->position) < 10.0f)
	{
		return;
	}
	float DistancefromThis = 0.0f;
	if (m_currentNode == nullptr)
	{
		setCurNode();
	}
	if (m_pTargetPathNode != AttackNode)
	{
		m_pTargetPathNode = AttackNode;
		if (m_currentNode == m_pTargetPathNode)
			return;
		PathManager::GetShortestPath(m_currentNode, m_pTargetPathNode);
		m_path = PathManager::getPath();
		m_currentNode = m_path.front()->GetFromNode();
		m_targetNode = m_path.front()->GetToNode();
		m_nodeIndex = 0;
	}
	//std::cout << m_targetNode->getTransform()->position.x << " " << m_targetNode->getTransform()->position.y << std::endl;
	MoveEnemy();
	if (abs(Util::distance(this->getTransform()->position, m_targetNode->getTransform()->position)) < 6.0f)
	{
		SetNextNode();
	}
	//std::cout << "finished move 2 attack" << std::endl;
}

void Warrior::Melee()
{
	if (m_meleeCounter < MELEECD)
		return;
	m_meleeCounter = 0;
	std::cout << "enter melee" << std::endl;
	std::cout << "Enemy: " << this->m_currentNode->getTransform()->position.x << " " << this->m_currentNode->getTransform()->position.y
		<< "Player: " << m_pTargetPlayer->getCurNode()->getTransform()->position.x << " " << m_pTargetPlayer->getCurNode()->getTransform()->position.y << std::endl;
	if (this->m_currentNode->getTransform()->position.x == m_pTargetPlayer->getCurNode()->getTransform()->position.x &&
		this->m_currentNode->getTransform()->position.y < m_pTargetPlayer->getCurNode()->getTransform()->position.y)
	{
		m_dir = down;
	}
	else if (this->m_currentNode->getTransform()->position.x == m_pTargetPlayer->getCurNode()->getTransform()->position.x &&
		this->m_currentNode->getTransform()->position.y > m_pTargetPlayer->getCurNode()->getTransform()->position.y)
	{
		m_dir = up;
	}
	else if (this->m_currentNode->getTransform()->position.x > m_pTargetPlayer->getCurNode()->getTransform()->position.x &&
		this->m_currentNode->getTransform()->position.y == m_pTargetPlayer->getCurNode()->getTransform()->position.y)
	{
		m_dir = left;
	}
	else if (this->m_currentNode->getTransform()->position.x < m_pTargetPlayer->getCurNode()->getTransform()->position.x &&
		this->m_currentNode->getTransform()->position.y == m_pTargetPlayer->getCurNode()->getTransform()->position.y)
	{
		m_dir = right;
	}

	SDL_Rect temp;
	switch (m_dir)
	{
	case up:
	{
		temp.x = this->getTransform()->position.x - 0.5f * this->getWidth();
		temp.y = this->getTransform()->position.y - 0.5f * this->getHeight() - MELEERANGE;
		temp.w = this->getWidth();
		temp.h = MELEERANGE;

		if (CollisionManager::AABBCheck(temp, m_pTargetPlayer))
		{
			m_pTargetPlayer->DecHP(MELEEDAMAGE);
			SoundManager::Instance().playSound("melee", 0, -1);
		}

		break;
	}
	case down:
	{
		temp.x = this->getTransform()->position.x - 0.5f * this->getWidth();
		temp.y = this->getTransform()->position.y + 0.5f * this->getHeight();
		temp.w = this->getWidth();
		temp.h = MELEERANGE;

		if (CollisionManager::AABBCheck(temp, m_pTargetPlayer))
		{
			m_pTargetPlayer->DecHP(MELEEDAMAGE);
			SoundManager::Instance().playSound("melee", 0, -1);
		}
		break;
	}
	case left:
	{
		temp.x = this->getTransform()->position.x - 0.5f * this->getWidth() - MELEERANGE;
		temp.y = this->getTransform()->position.y - 0.5f * this->getHeight();
		temp.w = MELEERANGE;
		temp.h = this->getHeight();

		if (CollisionManager::AABBCheck(temp, m_pTargetPlayer))
		{
			m_pTargetPlayer->DecHP(MELEEDAMAGE);
			SoundManager::Instance().playSound("melee", 0, -1);
		}
		break;
	}
	case right:
	{
		temp.x = this->getTransform()->position.x + 0.5f * this->getWidth();
		temp.y = this->getTransform()->position.y - 0.5f * this->getHeight();
		temp.w = MELEERANGE;
		temp.h = this->getHeight();

		if (CollisionManager::AABBCheck(temp, m_pTargetPlayer))
		{
			m_pTargetPlayer->DecHP(MELEEDAMAGE);
			SoundManager::Instance().playSound("melee", 0, -1);
		}
		break;
	}
	default: break;
	}
}

/*void Warrior::setPath(std::vector<PathConnection*> path)
{
	m_path = path;
	m_currentNode = m_path[0]->GetFromNode();
	m_targetNode = m_path[0]->GetToNode();
}*/

//std::vector<PathConnection*> Warrior::getPath()
//{
//	return m_path;
//}


//void Warrior::getDir()
//{
//	if (m_targetNode->getTransform()->position.x == m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y > m_currentNode-> getTransform()->position.y)
//		m_dir = down;
//	if (m_targetNode->getTransform()->position.x == m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y < m_currentNode->getTransform()->position.y)
//		m_dir = up;
//	if (m_targetNode->getTransform()->position.x < m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y == m_currentNode->getTransform()->position.y)
//		m_dir = left;
//	if (m_targetNode->getTransform()->position.x > m_currentNode->getTransform()->position.x && m_targetNode-> getTransform()->position.y == m_currentNode->getTransform()->position.y)
//		m_dir = right;
//	if (m_currentNode == m_targetNode)
//	{
//		PathNode* temp = start_point;
//		start_point = end_point;
//		end_point = temp;
//		temp = nullptr;
//		m_nodeIndex = 0;
//	}
//}

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

