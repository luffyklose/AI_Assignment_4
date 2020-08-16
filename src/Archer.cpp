#include "Archer.h"
#include <algorithm>
#include "ProjectileManager.h"
#include "TextureManager.h"
#include"Util.h"
#include "SoundManager.h"

const int SHOOTRANGE = 200;
const int SHOOTDAMAGE = 20;
const int SHOOTCD = 100;
const int DETECTRANGE = 300;

Archer::Archer(Player* player):Enemy(player)
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
	setType(ARCHER);

	this->m_pFiller = new HealthBarFiller(this);
	this->m_pBorder = new HealthBarBorder(this);

	m_detectionRadius = DETECTRANGE;
	m_accel = 0.2;
	m_velMax = 2.0;

	m_outerState = FIGHT;
	m_innerState = PATROL;

	M_withinShootRange = false;
	m_attackMode = false;

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
	//std::cout << "distance: " << Util::distance(this->getTransform()->position, m_pTargetPlayer->getTransform()->position) << std::endl;
	if (Util::distance(this->getTransform()->position, m_pTargetPlayer->getTransform()->position) < (float)SHOOTRANGE)
		M_withinShootRange = true;
	else
		M_withinShootRange = false;

	m_shootCounter++;
	detectPlayer(m_pTargetPlayer);

	std::cout <<"Before Update:"<< m_attackMode << " " << m_DetectPlayer << " " << m_hasLOS << std::endl;
	m_checkCurrentConditions();
	//std::cout << m_outerState << " " << m_innerState << std::endl;
	m_stateMachineUpdate();
	std::cout << "After Update:" << m_attackMode << " " << m_DetectPlayer << " " << m_hasLOS << std::endl;


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

void Archer::m_checkCurrentConditions()
{
	if (m_curHealth >= 25)
	{
		if(m_curHealth<100)
		{
			if(m_hasLOS)
			{
				m_innerState = WAIT_IN_COVER;
			}
			else
			{
				m_innerState = MOVE_TO_COVER;
			}
		}
		else 
		{
			if(m_attackMode)
			{
				if(m_hasLOS)
				{
					m_innerState = MOVE_TO_LOS;
				}
				else
				{
					if(M_withinShootRange)
					{
						m_innerState = RANGED_ATTACK;
					}
					else
					{
						m_innerState = MOVE_TO_RANGED;
					}
				}
			}
			else
			{
				if(m_DetectPlayer)
				{
					if(!m_hasLOS)
					{
						m_attackMode = true;
						std::cout << "set attack mode to " << m_attackMode<< std::endl;
						if(M_withinShootRange)
						{
							m_innerState = RANGED_ATTACK;
						}
						else
						{
							m_innerState = MOVE_TO_RANGED;
						}
					}
					else
					{
						m_innerState = MOVE_TO_LOS;
					}
				}
				else
				{
					//std::cout << m_attackMode << " " << m_DetectPlayer << " "<<m_hasLOS<<std::endl;
					m_innerState = PATROL;
				}
			}
		}
	}
	else
	{
		m_outerState = FLIGHT;
		//std::cout << "change 2 flee" << std::endl;
	}
}

void Archer::m_stateMachineUpdate()
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
				std::cout << "Patroling..." << std::endl;
				break;
			}
			case RANGED_ATTACK:
			{
				// Perform Range Attack Action
				Shoot();
				std::cout << "Shooting..." << std::endl;
				break;
			}
			case MOVE_TO_LOS:
			{
				// Move 2 LOS Action
				Move2LOS();
				//std::cout << "Moving to LOS..." << std::endl;
				break;
			}
			case MOVE_TO_RANGED:
			{
				// Move 2 Range Range Action
				Move2NearestAttackNode();
				std::cout << "Moving to Attack..." << std::endl;
				break;
			}
			case MOVE_TO_COVER:
			{
				break;
			}
			case WAIT_IN_COVER:
			{
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
	//std::cout << "Status: " << m_outerState << " " << m_innerState << std::endl;
}

void Archer::Move2NearestAttackNode()
{
	setCurNode();
	if (M_withinShootRange)
		return;
	float NodeDis = 0.0f;
	float BetweenDistance = Util::distance(m_pTargetPlayer->getTransform()->position, this->getTransform()->position);
	PathNode* TargetPathNode = nullptr;
	for (auto pathnode : NDMA::getPathNodeVec())
	{
		float ThisDistance = Util::distance(this->getTransform()->position, pathnode->getTransform()->position);
		float PlayerDistance = Util::distance(m_pTargetPlayer->getTransform()->position, pathnode->getTransform()->position);
		if (NodeDis == 0.0f && pathnode->getLOS() == false && PlayerDistance <=SHOOTRANGE-20)
		{
			NodeDis = ThisDistance;
			TargetPathNode = pathnode;
			//std::cout << "set distance as first value" << std::endl;
		}
		//std::cout << pathnode->getLOS() << " " << NodeDis << " " << tempDistance << std::endl;
		if (pathnode->getLOS() == false && PlayerDistance <= SHOOTRANGE-20 && ThisDistance < NodeDis)
		{
			NodeDis = ThisDistance;
			TargetPathNode = pathnode;
			//std::cout << TargetPathNode << std::endl;
			//std::cout << "this node: " << pathnode->getTransform()->position.x << " " << pathnode->getTransform()->position.y << std::endl;
			//std::cout << "set target at " << TargetPathNode->getTransform()->position.x << " " << TargetPathNode->getTransform()->position.y << std::endl;
		}
	}
	//std::cout << "distance:" << NodeDis << std::endl;
	if (Util::distance(this->getTransform()->position, TargetPathNode->getTransform()->position) < 6.0f)
	{
		return;
	}
	if (m_pTargetPathNode != TargetPathNode)
	{
		m_pTargetPathNode = TargetPathNode;
		if (m_currentNode == m_pTargetPathNode)
			return;
		PathManager::GetShortestPath(m_currentNode, m_pTargetPathNode);
		m_path = PathManager::getPath();
		m_currentNode = m_path.front()->GetFromNode();
		m_targetNode = m_path.front()->GetToNode();
		m_nodeIndex = 0;
	}
	MoveEnemy();
	if (abs(Util::distance(this->getTransform()->position, m_targetNode->getTransform()->position)) < 10.0f)
	{
		SetNextNode();
	}
	std::cout << "In Range? After: " << M_withinShootRange << std::endl;
}

void Archer::Shoot()
{
	if (m_shootCounter < SHOOTCD)
		return;
	m_shootCounter = 0;
	ProjectileManager::Instance()->generateFireball();
	auto fireball = ProjectileManager::Instance()->getFireBallList().back();
	setFaceDir();

	switch (m_dir)
	{
	case left:
		fireball->setDirection(Sprite::left);
		fireball->getTransform()->position.x = this->getTransform()->position.x;
		fireball->getTransform()->position.y = this->getTransform()->position.y + (float)this->getHeight() / 2;
		break;
	case right:
		fireball->setDirection(Sprite::right);
		fireball->getTransform()->position.x = this->getTransform()->position.x + (float)this->getWidth();
		fireball->getTransform()->position.y = this->getTransform()->position.y + (float)this->getHeight() / 2;
		break;
	case up:
		fireball->setDirection(Sprite::up);
		fireball->getTransform()->position.x = this->getTransform()->position.x + (float)this->getWidth() / 2;
		fireball->getTransform()->position.y = this->getTransform()->position.y;
		break;
	case down:
		fireball->setDirection(Sprite::down);
		fireball->getTransform()->position.x = this->getTransform()->position.x + (float)this->getWidth() / 2;
		fireball->getTransform()->position.y = this->getTransform()->position.y + (float)this->getHeight();
		break;
	default:
		break;
	}	
}

void Archer::setFaceDir()
{
	float Angle = atan2((this->getTransform()->position.x - m_pTargetPlayer->getTransform()->position.x), (this->getTransform()->position.y - m_pTargetPlayer->getTransform()->position.y)) * (float)180 / 3.1425926;
	if(Angle>=-45 && Angle<45)
	{
		setDirection(right);
	}
	else if(Angle>=45 && Angle<135)
	{
		setDirection(up);
	}
	else if(Angle>=135 && Angle<-135)
	{
		setDirection(left);
	}
	else if(Angle<-45 && Angle>=-135)
	{
		setDirection(down);
	}
	std::cout << "angle: " << Angle << " Dir: " << m_dir << std::endl;
}

//void Archer::MovePlane()
//{
//	switch (m_dir)
//	{
//	case left:
//	{
//		getRigidBody()->velocity.x += m_accel;
//		getRigidBody()->velocity.x = std::min(getRigidBody()->velocity.x, m_velMax);
//		getRigidBody()->velocity = glm::vec2(-m_vel, 0.0f);
//		break;
//	}
//	case right:
//	{
//		getRigidBody()->velocity.x += m_accel;
//		getRigidBody()->velocity.x = std::min(getRigidBody()->velocity.x, m_velMax);
//		getRigidBody()->velocity = glm::vec2(m_vel, 0.0f);
//		break;
//	}
//	case up:
//	{
//		getRigidBody()->velocity.y += m_accel;
//		getRigidBody()->velocity.y = std::min(getRigidBody()->velocity.y, m_velMax);
//		getRigidBody()->velocity = glm::vec2(0.0f, -m_vel);
//		break;
//	}
//	case down:
//	{
//		getRigidBody()->velocity.y += m_accel;
//		getRigidBody()->velocity.y = std::min(getRigidBody()->velocity.y, m_velMax);
//		getRigidBody()->velocity = glm::vec2(0.0f, m_vel);
//		break;
//	}
//	default:break;
//	}
//}

//void Archer::SetNextNode()
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


