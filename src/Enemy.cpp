﻿#include "Enemy.h"
#include <iostream>
#include "PathManager.h"
#include "Util.h"
#include"CollisionManager.h"
#include "ExplosionManager.h"
#include "NodeManager.h"
#include"PlayScene.h"

Enemy::Enemy(Player* player)
{
	//start_point = nullptr;
	//end_point = nullptr;
	//start_point->getTransform()->position = glm::vec2(0.0f,0.0f);
	//end_point->getTransform()->position = glm::vec2(0.0f, 0.0f);

	//m_isPatrol = false;
	m_curHealth = ENEMYMAXHEALTH;
	m_pTargetPlayer = player;
	m_pFleeNode = nullptr;
	m_isHitRecover = false;
	m_isFled = false;
	m_hitRecoverCounter = 0;
}

Enemy::~Enemy()
= default;

void Enemy::DecHP(int damage)
{
	m_curHealth = m_curHealth - damage;
	if (m_curHealth < 0)
	{
		m_curHealth = 0;
		ExplosionManager::Instance()->generateExplosion();
		auto explosion = ExplosionManager::Instance()->getExplosionList().back();
		explosion->getTransform()->position = this->getTransform()->position;
		reset();
	}
}

int Enemy::getDetectionRadius()
{
	return m_detectionRadius;
}


void Enemy::setPath(std::vector<PathNode*> PatrolPath)
{
	m_pPatrolPath = PatrolPath;
}

void Enemy::MovePlanetoPatrolNode()
{
	m_targetNode = m_pPatrolPath[m_nodeIndex];
	auto targetVector = Util::normalize(m_targetNode->getTransform()->position - this->getTransform()->position);
	//std::cout << "x: " << targetVector.x << " y:" << targetVector.y << std::endl;

	float buffer = 0.01;
	if (targetVector.x >= 1-buffer && targetVector.x<=1+buffer)
	{
		this->setAngle(90.0f);
	}
	else if (targetVector.x >= -1 - buffer && targetVector.x <= -1 + buffer)
	{
		this->setAngle(-90.0f);
	}
	else if (targetVector.y >= 1 - buffer && targetVector.y <= 1 + buffer)
	{
		this->setAngle(180.0f);
	}
	else if (targetVector.y >= -1 - buffer && targetVector.y <= -1 + buffer)
	{
		this->setAngle(0.0f);
	}

	this->getRigidBody()->velocity = targetVector;
	this->getTransform()->position += this->getRigidBody()->velocity * this->getRigidBody()->maxSpeed;
	if (abs(Util::distance(this->getTransform()->position, m_targetNode->getTransform()->position)) < 10.0f)
	{
		m_nodeIndex++;
		if (m_nodeIndex > m_pPatrolPath.size() - 1)
		{
			m_nodeIndex = 0;
		}
	}
}

void Enemy::addPathNode(PathNode* node)
{
	m_pPatrolPath.push_back(node);
}

void Enemy::detectPlayer(Sprite* player)
{
	m_DetectPlayer = CollisionManager::circleRectCheck(this->getTransform()->position, (float)m_detectionRadius, 
		player->getTransform()->position - glm::vec2(0.5f * player->getWidth(), 0.5f * player->getHeight()), player->getWidth(), player->getHeight());
	//std::cout <<"Detect: "<< m_DetectPlayer << std::endl;
}

std::vector<PathConnection*> Enemy::getPath()
{
	return m_path;
}

void Enemy::AddKeyNode(PathNode* keyNode)
{
	if(m_pKeyNodeVec.empty())
	{
		KeyNode* tempKeyNode = new KeyNode(keyNode);
		//std::cout << tempKeyNode->m_keyNode->getTransform()->position.x << " " << tempKeyNode->m_keyNode->getTransform()->position.y << std::endl;
		m_pKeyNodeVec.push_back(tempKeyNode);
		tempKeyNode = nullptr;
	}
	else
	{
		KeyNode* tempKeyNode = new KeyNode(keyNode, m_pKeyNodeVec.back(), m_pKeyNodeVec.front());
		//std::cout << tempKeyNode->m_keyNode->getTransform()->position.x << " " << tempKeyNode->m_keyNode->getTransform()->position.y << std::endl;
		m_pKeyNodeVec.back()->m_nextNode = tempKeyNode;
		m_pKeyNodeVec.front()->m_lastNode = tempKeyNode;
		m_pKeyNodeVec.push_back(tempKeyNode);
		tempKeyNode = nullptr;
	}
	//std::cout << m_pKeyNodeVec.size() << std::endl;
}

void Enemy::PatrolMove()
{
	if ((int)m_pKeyNodeVec.size() < 2)
	{
		//std::cout << "Key Node is smaller than 2" << std::endl;
		return;
	}
	if (m_path.empty())
	{
		//std::cout << m_curTargetKeyNode->m_keyNode->getTransform()->position.x << " " << m_curTargetKeyNode->m_keyNode->getTransform()->position.y << std::endl;
		//std::cout << m_curTargetKeyNode->m_nextNode->m_keyNode->getTransform()->position.x << " " << m_curTargetKeyNode->m_nextNode->m_keyNode->getTransform()->position.y << std::endl;
		PathManager::GetShortestPath(m_curTargetKeyNode->m_lastNode->m_keyNode, m_curTargetKeyNode->m_keyNode);
		m_path = PathManager::getPath();
		m_currentNode = m_curTargetKeyNode->m_lastNode->m_keyNode;
		m_targetNode = m_path.back()->GetToNode();
		m_nodeIndex = 0;
		std::cout << "Target:" << m_targetNode->getTransform()->position.x << " " << m_targetNode->getTransform()->position.y << std::endl;
	}

	if (abs(Util::distance(this->getTransform()->position, m_curTargetKeyNode->m_keyNode->getTransform()->position)) < 10.0f)
	{
		PathManager::GetShortestPath(m_curTargetKeyNode->m_keyNode, m_curTargetKeyNode->m_nextNode->m_keyNode);
		m_path = PathManager::getPath();
		m_curTargetKeyNode = m_curTargetKeyNode->m_nextNode;
		m_currentNode = m_curTargetKeyNode->m_keyNode;
		m_targetNode = m_path.front()->GetToNode();
		std::cout << "To: " << m_curTargetKeyNode->m_keyNode->getTransform()->position.x << " " << m_curTargetKeyNode->m_keyNode->getTransform()->position.y << std::endl;
		m_nodeIndex = 0;
	}

	MoveEnemy();

	if (abs(Util::distance(this->getTransform()->position, m_targetNode->getTransform()->position)) < 10.0f)
	{
		SetNextNode();
	}
}

void Enemy::SetNextNode()
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

void Enemy::Flee()
{
	if (NDMA::getFleeNodeVec().empty())
		return;
	if (m_currentNode == nullptr)
	{
		setCurNode();
	}
	int LeastCost = 0;
	if (m_pFleeNode == nullptr)
	{
		for (auto pathnode : NDMA::getFleeNodeVec())
		{
			if (m_currentNode == pathnode)
				return;
			PathManager::GetShortestPath(m_currentNode, pathnode);
			int tempCost = PathManager::getCost();
			if (LeastCost == 0 || tempCost < LeastCost)
			{
				m_pFleeNode = pathnode;
				LeastCost = tempCost;
				m_path = PathManager::getPath();
				m_currentNode = m_path.front()->GetFromNode();
				m_targetNode = m_path.front()->GetToNode();
				m_nodeIndex = 0;
			}
		}
	}
	std::cout << "flee to " << m_pFleeNode->getTransform()->position.x << " " << m_pFleeNode->getTransform()->position.y << std::endl;
	if (Util::distance(this->getTransform()->position, m_pFleeNode->getTransform()->position) < 6.0f)
	{
		m_isFled = true;
		reset();
		return;
	}
	if (m_pTargetPathNode != m_pFleeNode)
	{
		m_pTargetPathNode = m_pFleeNode;
		if (m_currentNode == m_pTargetPathNode)
		{			
			return;
		}			
		PathManager::GetShortestPath(m_currentNode, m_pTargetPathNode);
		m_path = PathManager::getPath();
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

void Enemy::Move2LOS()
{
	setCurNode();
	if (!m_hasLOS)
		return;
	float NodeDis = 0.0f;
	PathNode* TargetPathNode = nullptr;
	for (auto pathnode : NDMA::getPathNodeVec())
	{
		float tempDistance = Util::distance(this->getTransform()->position, pathnode->getTransform()->position);
		if (NodeDis == 0.0f && pathnode->getLOS() == false)
		{
			NodeDis = tempDistance;
			TargetPathNode = pathnode;
		}
		//std::cout << pathnode->getLOS() << " " << NodeDis << " " << tempDistance << std::endl;
		if (pathnode->getLOS() == false && tempDistance < NodeDis)
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

void Enemy::MoveEnemy()
{
	setDir();
	auto targetVector = Util::normalize(m_targetNode->getTransform()->position - this->getTransform()->position);
	//std::cout << "x: " << targetVector.x << " y:" << targetVector.y << std::endl;

	float buffer = 0.01;
	if (targetVector.x >= 1 - buffer && targetVector.x <= 1 + buffer)
	{
		this->setAngle(90.0f);
	}
	else if (targetVector.x >= -1 - buffer && targetVector.x <= -1 + buffer)
	{
		this->setAngle(-90.0f);
	}
	else if (targetVector.y >= 1 - buffer && targetVector.y <= 1 + buffer)
	{
		this->setAngle(180.0f);
	}
	else if (targetVector.y >= -1 - buffer && targetVector.y <= -1 + buffer)
	{
		this->setAngle(0.0f);
	}

	this->getRigidBody()->velocity = targetVector;
	this->getTransform()->position += this->getRigidBody()->velocity * this->getRigidBody()->maxSpeed;
}

void Enemy::setCurNode()
{
	float tempDis = 0.0f;
	for (auto pathnode : NDMA::getPathNodeVec())
	{
		float tempDistance = Util::distance(this->getTransform()->position, pathnode->getTransform()->position);
		if (tempDis == 0.0f)
		{
			tempDis = tempDistance;
			m_currentNode = pathnode;
		}
		else if (tempDistance < tempDis)
		{
			tempDis = tempDistance;
			m_currentNode = pathnode;
		}
	}
}

void Enemy::setDir()
{
	if (m_targetNode == nullptr || m_currentNode == nullptr)
		return;
	else if (m_targetNode->getTransform()->position.x == m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y > m_currentNode->getTransform()->position.y)
		m_dir = down;
	else if (m_targetNode->getTransform()->position.x == m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y < m_currentNode->getTransform()->position.y)
		m_dir = up;
	else if (m_targetNode->getTransform()->position.x < m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y == m_currentNode->getTransform()->position.y)
		m_dir = left;
	else if (m_targetNode->getTransform()->position.x > m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y == m_currentNode->getTransform()->position.y)
		m_dir = right;
}

void Enemy::setCurTargetKdyNode(KeyNode* target)
{
	m_curTargetKeyNode = target;
}

void Enemy::setActive()
{
	m_bIsActive = true;
}
