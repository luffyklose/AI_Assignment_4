#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__
//#include "../Template/Fireball.h"
#include"Fireball.h"

const int MAXENEMY = 1;

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "DestructibleObstacle.h"
#include "PathNode.h"
#include "Tile.h"
#include <array>

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	static std::vector<PathNode*> getNodeVec() { return m_pathNodeVec; }
private:
	void buildGrid();
	void displayGrid();
	void drawLOS();
	void setGridLOS();
	void QueueLine(const glm::vec2 start, const glm::vec2 end, const glm::vec4 col);
	void DrawLine();
	void DrawPath();
	void LoadMap();
	void AddConnection();
	PathNode* getPathNode(int x, int y);
	void RemoveNullObject();
public:
	
private:	
	glm::vec2 m_mousePosition;

	//Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	
	bool m_isDebugMode;
	bool m_bPlayerHasLOS;

	bool m_pHPressed,
		m_pPPressed,
		m_pKPressed,
		m_pJPressed,
		m_pLeftButtonPressed,
		m_pRightButtonPressed;

	int m_pathNodeNum;

	//Obstacle* m_pObstacle;
	static std::vector<PathNode*> m_pathNodeVec;

	//DestructibleObstacle* m_pObstacle;

	std::vector<Enemy*> m_enemyVec;
	//std::vector<DestructibleObstacle*> m_obstacleVec;
	std::vector<glm::vec2> m_pointVec;
	std::vector<glm::vec4> m_colorVec;
	std::array<std::array<Tile*, Config::COL_NUM>, Config::ROW_NUM> m_level;
	std::vector<Tile*> m_obstacleVec;
	
	Label* m_pScoreLabel;
	Label* m_pEnemyKilledLabel;

	//int m_enemyKilled{};
};



#endif /* defined (__PLAY_SCENE__) */
