#pragma once
#include "Sprite.h"
#include"PathNode.h"
#include "PathManager.h"

const int ENEMYMAXHEALTH = 100;

class Enemy:public Sprite
{
public:
	Enemy();
	~Enemy();
	
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void clean() = 0;
    virtual void reset() = 0;
	virtual void setActive() = 0;

	//getter
	int getDetectionRadius();
	bool getPatrolMode() { return m_isPatrol; }
	bool getHasLOS() { return m_hasLOS; };
	virtual int getMaxhealth() { return ENEMYMAXHEALTH; }
	std::vector<KeyNode*> getKeyNode() { return m_pKeyNodeVec; }
	KeyNode* getCurTargetKeyNode() { return m_curTargetKeyNode; }
	std::vector<PathNode*> getPatrolPath() { return m_pPatrolPath; }
	bool getDetect() { return m_DetectPlayer; }
	std::vector<PathConnection*> getPath();
	
	//setter
	void setStartNode(PathNode* start) { start_point = start; }
	void setEndNode(PathNode* end) { end_point = end; }
	void setPath(std::vector<PathNode*> PatrolPath);
	void setPatrolMode(bool isPatrol) { m_isPatrol = isPatrol; }
	void setHasLOS(bool hasLOS) { m_hasLOS = hasLOS; }
	void setCurTargetKdyNode(KeyNode* target);
	
	void DecHP(int damage);	
	void MovePlanetoPatrolNode();	
	void addPathNode(PathNode* node);	
	void detectPlayer(Sprite* player);	
	void AddKeyNode(PathNode* keyNode);
	void PatrolMove();

	void SetNextNode();	
	void Move2LOS();
	void MoveEnemy();
  bool isActive() { return m_bIsActive; }
protected:
	
public:

protected:
	float 	m_accel,
			m_vel,
			m_velMax;
	int m_nodeIndex;	
	int	m_detectionRadius;
	bool m_isPatrol,
		m_hasLOS,
		m_DetectPlayer,
		m_bIsActive;
	
	PathNode* start_point, * end_point;
	PathNode* m_targetNode,
		* m_currentNode,
		* m_nextNode;
	PathNode* m_pTargetPathNode;
	KeyNode* m_curTargetKeyNode;
	std::vector<PathConnection*> m_path;
	std::vector<PathNode*> m_pPatrolPath;
	std::vector<KeyNode*> m_pKeyNodeVec;
};
