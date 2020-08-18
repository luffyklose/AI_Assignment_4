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
const int ARCHERHITRECOVERTIME = 80;
const float MAXSPEED = 2.0f;

Archer::Archer(Player* player):Enemy(player)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/archer.txt",
		"../Assets/sprites/archer.png",
		"archer");

	/*setSpriteSheet(TextureManager::Instance()->getSpriteSheet("plane"));*/
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("archer"));

	// set frame width
	setWidth(40);

	// set frame height
	setHeight(40);

	//alt pending
	//getTransform()->position = glm::vec2(x,y);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->maxSpeed = MAXSPEED;
	getRigidBody()->isColliding = false;
	setType(ARCHER);

	this->m_pFiller = new HealthBarFiller(this);
	this->m_pBorder = new HealthBarBorder(this);

	m_detectionRadius = DETECTRANGE;
	m_accel = 0.2;
	m_velMax = 2.0;
	m_hitRecoverCounter = ARCHERHITRECOVERTIME;

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

	/*if (m_isPatrol)
	{
		TextureManager::Instance()->playAnimation(
			"archer", getAnimation("walk_right"),
			x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
	}
	else
	{
		TextureManager::Instance()->playAnimation(
			"archer", getAnimation("idle"),
			x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
	}*/

	if (m_outerState == HITRECOVER)
	{
		//std::cout << "be hit: " << m_outerState << " " << m_dir << std::endl;
		switch (m_dir)
		{
		case left:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("be_hit_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			//std::cout << "be hit left" << std::endl;
			break;
		case right:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("be_hit_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			//std::cout << "be hit right" << std::endl;
		case up:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("be_hit_up"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			//std::cout << "be hit up" << std::endl;
			break;
		case down:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("be_hit_up"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			//std::cout << "be hit down" << std::endl;
			break;
		default:break;
		}
	}
	else if (m_outerState == FLIGHT || m_innerState == MOVE_TO_RANGED || m_innerState == MOVE_TO_LOS || m_innerState == PATROL || m_innerState == MOVE_TO_COVER)
	{
		switch (m_dir)
		{
		case left:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("walk_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true, SDL_FLIP_HORIZONTAL);
			break;
		case right:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("walk_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			break;
		case up:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("walk_up"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			break;
		case down:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("walk_down"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			break;
		default:break;
		}
	}
	else if (m_innerState == WAIT_IN_COVER)
	{
		if(m_dir == right || m_dir == up)
		{
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("idle"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
		}
		else if (m_dir == left || m_dir == down)
		{
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("idle"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		}
	}
	else if (m_innerState == RANGED_ATTACK)
	{
		switch (m_dir)
		{
		case left:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("hit_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true, SDL_FLIP_HORIZONTAL);
			break;
		case right:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("hit_right"),
				x, y, getWidth(), getHeight(), 0.1f, 0, 255, true);
			break;
		case up:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("hit_right"),
				x, y, getWidth(), getHeight(), 0.1f, -90, 255, true);
			break;
		case down:
			TextureManager::Instance()->playAnimation(
				"archer", getAnimation("hit_right"),
				x, y, getWidth(), getHeight(), 0.1f, 90, 255, true);
			break;
		default:break;
		}
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

	//std::cout <<"Before Update:"<< m_attackMode << " " << m_DetectPlayer << " " << m_hasLOS << std::endl;
	m_checkCurrentConditions();
	//std::cout << m_outerState << " " << m_innerState << std::endl;
	m_stateMachineUpdate();
	//std::cout << "After Update:" << m_attackMode << " " << m_DetectPlayer << " " << m_hasLOS << std::endl;


	m_pFiller->update();
	m_pBorder->update();
}

void Archer::clean()
{
}

void Archer::reset()
{
	getTransform()->position.x = -1000;
	getTransform()->position.y = -1000;
	m_bIsActive = false;
}

void Archer::setActive()
{
	m_bIsActive = true;
	m_outerState = FIGHT;
	m_innerState = PATROL;

	M_withinShootRange = false;
	m_attackMode = false;

	m_curHealth = ENEMYMAXHEALTH;
	m_isHitRecover = false;
	m_isFled = false;
	m_hitRecoverCounter = ARCHERHITRECOVERTIME;
	m_dir = right;
}

void Archer::m_buildAnimations()
{
	/*Animation idleAnimation = Animation();

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

	setAnimation(runAnimation);*/

	Animation idleAnimation = Animation();
	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_idle_0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_idle_1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_idle_2"));
	setAnimation(idleAnimation);

	Animation walk_rightAnimation = Animation();
	walk_rightAnimation.name = "walk_right";
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_right_0"));
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_right_1"));
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_right_2"));
	walk_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_right_3"));
	setAnimation(walk_rightAnimation);

	Animation walk_upAnimation = Animation();
	walk_upAnimation.name = "walk_up";
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_up_0"));
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_up_1"));
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_up_2"));
	walk_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_up_3"));
	setAnimation(walk_upAnimation);

	Animation walk_downAnimation = Animation();
	walk_downAnimation.name = "walk_down";
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_down_0"));
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_down_1"));
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_down_2"));
	walk_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_walk_down_3"));
	setAnimation(walk_downAnimation);

	Animation hit_rightAnimation = Animation();
	hit_rightAnimation.name = "hit_right";
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_right_0"));
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_right_1"));
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_right_2"));
	hit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_right_3"));
	setAnimation(hit_rightAnimation);

	Animation hit_upAnimation = Animation();
	hit_upAnimation.name = "hit_up";
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_up_0"));
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_up_1"));
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_up_2"));
	hit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_up_3"));
	setAnimation(hit_upAnimation);

	Animation hit_downAnimation = Animation();
	hit_downAnimation.name = "hit_down";
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_down_0"));
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_down_1"));
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_down_2"));
	hit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_hit_down_3"));
	setAnimation(hit_downAnimation);

	Animation behit_rightAnimation = Animation();
	behit_rightAnimation.name = "be_hit_right";
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_right_0"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_right_1"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_right_2"));
	behit_rightAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_right_3"));
	setAnimation(behit_rightAnimation);

	Animation behit_upAnimation = Animation();
	behit_upAnimation.name = "be_hit_up";
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_up_0"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_up_1"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_up_2"));
	behit_upAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_up_3"));
	setAnimation(behit_upAnimation);

	Animation behit_downAnimation = Animation();
	behit_upAnimation.name = "be_hit_down";
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_down_0"));
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_down_1"));
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_down_2"));
	behit_downAnimation.frames.push_back(getSpriteSheet()->getFrame("archer_behit_down_3"));
	setAnimation(behit_downAnimation);
}

void Archer::m_checkCurrentConditions()
{
	if (m_hitRecoverCounter < ARCHERHITRECOVERTIME)
	{
		m_outerState = HITRECOVER;
		//std::cout << "dir: " << m_dir << std::endl;
	}
	else if (m_curHealth >= 25)
	{
		m_outerState = FIGHT;
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
						//std::cout << "set attack mode to " << m_attackMode<< std::endl;
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
				//std::cout << "Patroling..." << std::endl;
				break;
			}
			case RANGED_ATTACK:
			{
				// Perform Range Attack Action
				Shoot();
				//std::cout << "Shooting..." << std::endl;
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
				//std::cout << "Moving to Attack..." << std::endl;
				break;
			}
			case MOVE_TO_COVER:
			{
				Move2Cover();
				break;
			}
			case WAIT_IN_COVER:
			{
				break;
			}
		}
			break;
		case HITRECOVER:
		{
			m_hitRecoverCounter++;
			break;
		}
		//case DEATH:
		//{
		//	break;
		//}
		case FLIGHT:
		{
			// Flee Action
			Flee();
			//std::cout << "Fleeing..." << std::endl;
			break;
		}
		break;
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
		if (m_path.empty())
		{
			std::cout << "cur: " << m_currentNode->getTransform()->position.x << " " << m_currentNode->getTransform()->position.y << std::endl;;
			std::cout << "target: " << m_pTargetPathNode->getTransform()->position.x << " " << m_pTargetPathNode->getTransform()->position.y << std::endl;
			return;
		}
		m_currentNode = m_path.front()->GetFromNode();
		m_targetNode = m_path.front()->GetToNode();
		m_nodeIndex = 0;
	}
	MoveEnemy();
	if (abs(Util::distance(this->getTransform()->position, m_targetNode->getTransform()->position)) < 10.0f)
	{
		SetNextNode();
	}
	//std::cout << "In Range? After: " << M_withinShootRange << std::endl;
}

void Archer::Shoot()
{
	if (m_shootCounter < SHOOTCD)
		return;
	m_shootCounter = 0;
	ProjectileManager::Instance()->generateFireball();
	auto fireball = ProjectileManager::Instance()->getFireBallList().back();
	ProjectileManager::Instance()->getEnemyFireVec().push_back(fireball);
	auto FireBallVelVector = Util::normalize(m_pTargetPlayer->getTransform()->position - this->getTransform()->position);
	FireBallVelVector.x *= fireball->getSpeed();
	FireBallVelVector.y *= fireball->getSpeed();
	fireball->getRigidBody()->velocity = FireBallVelVector;
	
	setFaceDir();
	switch (m_dir)
	{
	case left:
		fireball->setDirection(Sprite::left);
		fireball->getTransform()->position.x = this->getTransform()->position.x - 0.5f * this->getWidth() - 0.5f * fireball->getWidth();
		fireball->getTransform()->position.y = this->getTransform()->position.y ;
		break;
	case right:
		fireball->setDirection(Sprite::right);
		fireball->getTransform()->position.x = this->getTransform()->position.x + 0.5f * this->getWidth() + 0.5f * fireball->getWidth();
		fireball->getTransform()->position.y = this->getTransform()->position.y ;
		break;
	case up:
		fireball->setDirection(Sprite::up);
		fireball->getTransform()->position.x = this->getTransform()->position.x;
		fireball->getTransform()->position.y = this->getTransform()->position.y - 0.5f * this->getHeight() - 0.5f * fireball->getHeight();
		break;
	case down:
		fireball->setDirection(Sprite::down);
		fireball->getTransform()->position.x = this->getTransform()->position.x ;
		fireball->getTransform()->position.y = this->getTransform()->position.y + 0.5f * this->getHeight() + 0.5f * fireball->getHeight();
		break;
	default:
		break;
	}
	std::cout << "archer shoots fireball" << std::endl;
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
	//sstd::cout << "angle: " << Angle << " Dir: " << m_dir << std::endl;
}

void Archer::Move2Cover()
{
	setCurNode();
	if (m_currentNode->getLOS())
		return;
	float NodeDis = 0.0f;
	PathNode* TargetPathNode = nullptr;
	for (auto pathnode : NDMA::getPathNodeVec())
	{
		if (!pathnode->getLOS())
			continue;
		float tempDistance = Util::distance(this->getTransform()->position, pathnode->getTransform()->position);
		if (NodeDis == 0.0f || tempDistance < NodeDis)
		{
			NodeDis = tempDistance;
			TargetPathNode = pathnode;
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
		if (m_path.empty())
		{
			std::cout << "cur: " << m_currentNode->getTransform()->position.x << " " << m_currentNode->getTransform()->position.y << std::endl;;
			std::cout << "target: " << m_pTargetPathNode->getTransform()->position.x << " " << m_pTargetPathNode->getTransform()->position.y << std::endl;
			return;
		}
		m_currentNode = m_path.front()->GetFromNode();
		m_targetNode = m_path.front()->GetToNode();
		m_nodeIndex = 0;
	}
	MoveEnemy();
	if (abs(Util::distance(this->getTransform()->position, m_targetNode->getTransform()->position)) < 6.0f)
	{
		SetNextNode();
	}
}

