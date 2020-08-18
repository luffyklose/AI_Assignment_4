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
const int WARRIORHITRECOVERTIME = 50;
const float MAXSPEED = 2.0f;

Warrior::Warrior(Player* player):Enemy(player)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/warrior.txt",
		"../Assets/sprites/warrior.png",
		"warrior");
	
	/*setSpriteSheet(TextureManager::Instance()->getSpriteSheet("plane"));*/
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("warrior"));

	// set frame width
	setWidth(40);
	
	// set frame height
	setHeight(40);

	//alt pending
	//getTransform()->position = glm::vec2(-1000,-1000);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	getRigidBody()->maxSpeed = MAXSPEED;
	setType(WARRIOR);

	this->m_pFiller = new HealthBarFiller(this);
	this->m_pBorder = new HealthBarBorder(this);
	
	m_detectionRadius = DETECTRANGE;
	m_accel = 0.2;
	m_velMax = 2.0;
	m_hitRecoverCounter = WARRIORHITRECOVERTIME;
	
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
	if (m_outerState == HITRECOVER)
	{
		std::cout << "be hit: " << m_outerState << " " << m_dir << std::endl;
		switch (m_dir)
		{
		case left:
			TextureManager::Instance()->playAnimation(
				"warrior", getAnimation("be_hit_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true, SDL_FLIP_HORIZONTAL);
			//std::cout << "be hit left" << std::endl;
			break;
		case right:
			TextureManager::Instance()->playAnimation(
				"warrior", getAnimation("be_hit_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			std::cout << "be hit right" << std::endl;
		case up:
			TextureManager::Instance()->playAnimation(
				"warrior", getAnimation("be_hit_up"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			//std::cout << "be hit up" << std::endl;
			break;
		case down:
			TextureManager::Instance()->playAnimation(
				"warrior", getAnimation("be_hit_up"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			//std::cout << "be hit down" << std::endl;
			break;
		default:break;
		}
	}
	else if (m_outerState == FLIGHT || m_innerState == MOVE_TO_MELEE || m_innerState == MOVE_TO_LOS || m_innerState == PATROL)
	{
		switch (m_dir)
		{
		case left:
			TextureManager::Instance()->playAnimation(
				"warrior", getAnimation("walk_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true,SDL_FLIP_HORIZONTAL);
			//std::cout << "walk left" << std::endl;
			break;
		case right:
			TextureManager::Instance()->playAnimation(
				"warrior", getAnimation("walk_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			//std::cout << "walk right" << std::endl;
			break;
		case up:
			TextureManager::Instance()->playAnimation(
				"warrior", getAnimation("walk_up"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			//std::cout << "walk up" << std::endl;
			break;
		case down:
			TextureManager::Instance()->playAnimation(
				"warrior", getAnimation("walk_down"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			//std::cout << "walk down" << std::endl;
			break;
		default:break;
		}
	}
	else if (m_innerState == MELEE_ATTACK)
	{
		switch (m_dir)
		{
		case left:
			TextureManager::Instance()->playAnimation(
			"warrior", getAnimation("hit_right"),
			x, y, getWidth(), getHeight(), 0.1f, 0, 255, true,SDL_FLIP_HORIZONTAL);
			break;
		case right:
			TextureManager::Instance()->playAnimation(
				"warrior", getAnimation("hit_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			break;
		case up:
			TextureManager::Instance()->playAnimation(
				"warrior", getAnimation("hit_up"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			break;
		case down:
			TextureManager::Instance()->playAnimation(
				"warrior", getAnimation("hit_down"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			break;
		default:break;
		}
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

	detectPlayer(m_pTargetPlayer);
	m_checkCurrentConditions();
	//std::cout << m_outerState << " " << m_innerState << std::endl;
	m_stateMachineUpdate();
		
	m_meleeCounter++;

	m_pFiller->update();
	m_pBorder->update();

	//getTransform()->position += getRigidBody()->velocity;
	//getRigidBody()->velocity *= getRigidBody()->velocity * 0.9f;
}

void Warrior::clean()
{
}

void Warrior::reset()
{
	getTransform()->position.x = -1000;
	getTransform()->position.y = -1000;
	m_bIsActive = false;
}

void Warrior::setActive()
{
	m_bIsActive = true;
	m_outerState = FIGHT;
	m_innerState = PATROL;

	m_withinMeleeRange = false;
	m_attackMode = false;
	m_curHealth = ENEMYMAXHEALTH;
	m_isHitRecover = false;
	m_isFled = false;
	m_hitRecoverCounter = WARRIORHITRECOVERTIME;
	m_dir = right;
}

void Warrior::m_buildAnimations()
{
	Animation idleAnimation = Animation();
	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_idle_0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_idle_1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_idle_2"));
	setAnimation(idleAnimation);

	Animation walk_rightAnimation = Animation();
	walk_rightAnimation.name = "walk_right";
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_right_0"));
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_right_1"));
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_right_2"));
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_right_3"));
	setAnimation(walk_rightAnimation);

	Animation walk_upAnimation = Animation();
	walk_upAnimation.name = "walk_up";
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_up_0"));
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_up_1"));
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_up_2"));
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_up_3"));
	setAnimation(walk_upAnimation);

	Animation walk_downAnimation = Animation();
	walk_downAnimation.name = "walk_down";
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_down_0"));
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_down_1"));
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_down_2"));
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_walk_down_3"));
	setAnimation(walk_downAnimation);

	Animation hit_rightAnimation = Animation();
	hit_rightAnimation.name = "hit_right";
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_right_0"));
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_right_1"));
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_right_2"));
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_right_3"));
	setAnimation(hit_rightAnimation);

	Animation hit_upAnimation = Animation();
	hit_upAnimation.name = "hit_up";
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_up_0"));
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_up_1"));
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_up_2"));
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_up_3"));
	setAnimation(hit_upAnimation);

	Animation hit_downAnimation = Animation();
	hit_downAnimation.name = "hit_down";
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_down_0"));
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_down_1"));
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_down_2"));
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_hit_down_3"));
	setAnimation(hit_downAnimation);

	Animation behit_rightAnimation = Animation();
	behit_rightAnimation.name = "be_hit_right";
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_right_0"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_right_1"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_right_2"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_right_3"));
	setAnimation(behit_rightAnimation);

	Animation behit_upAnimation = Animation();
	behit_upAnimation.name = "be_hit_up";
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_up_0"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_up_1"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_up_2"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_up_3"));
	setAnimation(behit_upAnimation);

	Animation behit_downAnimation = Animation();
	behit_upAnimation.name = "be_hit_below";
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_down_0"));
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_down_1"));
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_down_2"));
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("warrior_behit_down_3"));
	setAnimation(behit_downAnimation);
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



void Warrior::m_checkCurrentConditions()
{
	if((m_hitRecoverCounter < WARRIORHITRECOVERTIME))
	{
		m_outerState = HITRECOVER;
	}
	else if (m_curHealth >= 25)
	{
		m_outerState = FIGHT;
		if (m_attackMode)
		{
			if (!m_hasLOS)
			{
				// check the distance
				//std::cout << "In Melee Range: " << m_withinMeleeRange << std::endl;
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
	case HITRECOVER:
	{
		m_hitRecoverCounter++;
		break;
	}
	/*case DEATH:
	{
		break;
	}*/
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
			m_pTargetPlayer->setPlayerState(PLAYER_BEHIT);
			m_pTargetPlayer->resetHitRecover();
			SoundManager::Instance().playSound("melee", 0, 1);
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
			m_pTargetPlayer->setPlayerState(PLAYER_BEHIT);
			m_pTargetPlayer->resetHitRecover();
			SoundManager::Instance().playSound("melee", 0, 1);
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
			m_pTargetPlayer->setPlayerState(PLAYER_BEHIT);
			m_pTargetPlayer->resetHitRecover();
			SoundManager::Instance().playSound("melee", 0, 1);
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
			m_pTargetPlayer->setPlayerState(PLAYER_BEHIT);
			m_pTargetPlayer->resetHitRecover();
			SoundManager::Instance().playSound("melee", 0, 1);
		}
		break;
	}
	default: break;
	}
}
