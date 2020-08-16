#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"
#include"PathManager.h"
#include <fstream>
#include "ProjectileManager.h"
#include"SoundManager.h"
#include "EnemyManager.h"

std::vector<PathNode*> PlayScene::m_pathNodeVec;

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();

	if(m_isDebugMode)
	{
		Util::DrawRect(m_pPlayer->getTransform()->position , m_pPlayer->getWidth(), m_pPlayer->getHeight());

		for(auto m_pEnemy:m_enemyVec)
		{
			auto LOSColour = (m_pEnemy->getHasLOS()) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			auto CircleColour = (m_pEnemy->getDetect()) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			Util::DrawLine(m_pPlayer->getTransform()->position, m_pEnemy->getTransform()->position, LOSColour);
			Util::DrawRect(m_pEnemy->getTransform()->position - glm::vec2(0.5f*m_pEnemy->getWidth(),0.5f*m_pEnemy->getHeight()) , m_pEnemy->getWidth(), m_pEnemy->getHeight());
			Util::DrawCircle(m_pEnemy->getTransform()->position , m_pEnemy->getDetectionRadius(),CircleColour);
		}

		/*for(auto m_pObstacle:m_obstacleVec)
		{
			Util::DrawRect(m_pObstacle->getTransform()->position - glm::vec2(m_pObstacle->getWidth() * 0.5f, m_pObstacle->getHeight() * 0.5f), m_pObstacle->getWidth(), m_pObstacle->getHeight());

		}*/

		displayGrid();
		
		//drawLOS();

		//DrawLine();
		//DrawPath();
	}
	
}

void PlayScene::update()
{
	updateDisplayList();

	/*m_enemyKilled = MAXENEMY - m_enemyVec.size();
	m_pEnemyKilledLabel->setText(std::to_string(m_enemyKilled));*/
	
	//std::cout << "DestructibleObstacle Number: " << m_obstacleVec.size() << std::endl;
	/*for (auto fireball : m_pFireballVec)
	{
		for (auto enemy : m_enemyVec)
		{
			if (CollisionManager::AABBCheck(fireball, enemy))
			{
				enemy->DecHP(m_pPlayer->getDamage());
				delete fireball;
				fireball = nullptr;
			}
		}
	}*/
	
	for(auto m_pEnemy : m_enemyVec)
	{
		m_pEnemy->detectPlayer(m_pPlayer);
		for (int i=0;i<(int)m_obstacleVec.size();i++)
		{
			m_pEnemy->setHasLOS(CollisionManager::LOSCheck(m_pPlayer, m_pEnemy, m_obstacleVec[i]));
			if(m_pEnemy->getHasLOS())
			{
				break;				
			}
		}

		/*if(m_pEnemy->getCurHealth()<=0)
		{
			delete m_pEnemy;
			m_pEnemy = nullptr;
		}*/
	}

	


	//std::cout << "Player: " << m_pPlayer->getTransform()->position.x << " " << m_pPlayer->getTransform()->position.y << std::endl;
	setGridLOS();
	//CollisionManager::CheckMapCollision(m_pPlayer, m_obstacleVec);
	//std::cout << "HP: " << m_enemyVec[0]->getCurHealth() << std::endl;
	
	RemoveNullObject();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	/*if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone && !CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(-5.0f, 0.0f), m_obstacleVec) && m_pPlayer->getTransform()->position.x > 0.0f)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);

				m_pPlayer->getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
				m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
				m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
				SoundManager::Instance().playSound("step", 0, -1);
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone && !CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(5.0f, 0.0f), m_obstacleVec) && m_pPlayer->getTransform()->position.x < Config::SCREEN_WIDTH - m_pPlayer->getWidth())
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);

				m_pPlayer->getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
				m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
				m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
				SoundManager::Instance().playSound("step", 0, -1);
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}*/


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A) && !CollisionManager::PlayerCollision(m_pPlayer,glm::vec2(-5.0f,0.0f),m_obstacleVec) && m_pPlayer->getTransform()->position.x > 0.0f)
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_pPlayer->setDirection(Sprite::left);

			m_pPlayer->getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("step", 0, -1);
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D) && !CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(5.0f, 0.0f), m_obstacleVec) && m_pPlayer->getTransform()->position.x < Config::SCREEN_WIDTH - m_pPlayer->getWidth())
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_pPlayer->setDirection(Sprite::right);

			m_pPlayer->getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("step", 0, -1);
		}
		else if(EventManager::Instance().isKeyDown(SDL_SCANCODE_W) && !CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(0.0f, -5.0f), m_obstacleVec) && m_pPlayer->getTransform()->position.y > 0.0f)
		{
			m_pPlayer->setDirection(Sprite::up);
			
			m_pPlayer->getRigidBody()->velocity = glm::vec2(0.0f, -5.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("step", 0, -1);
		}
		else if(EventManager::Instance().isKeyDown(SDL_SCANCODE_S) && !CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(0.0f, 5.0f), m_obstacleVec) && m_pPlayer->getTransform()->position.y < Config::SCREEN_HEIGHT - m_pPlayer->getHeight())
		{
			m_pPlayer->setDirection(Sprite::down);
			
			m_pPlayer->getRigidBody()->velocity = glm::vec2(0.0f, 5.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("step", 0, -1);
		}
		
		switch (m_pPlayer->getDirection())
		{
		case Sprite::left:
			m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			break;
		case Sprite::right:
			m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			break;
		case Sprite::up:
			m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			break;
		case Sprite::down:
			m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			break;
		default:
			break;
		}
	}

	if(!m_pRightButtonPressed)
	{
		if(EventManager::Instance().getMouseButton(2))
		{
			m_pRightButtonPressed = true;

			auto fireball = ProjectileManager::Instance()->generateFireball();
			addChild(fireball);
			
			switch (m_pPlayer->getDirection())
			{
			case 1:
				fireball->setDirection(Sprite::left);
				fireball->getTransform()->position.x = m_pPlayer->getTransform()->position.x;
				fireball->getTransform()->position.y = m_pPlayer->getTransform()->position.y + (float)m_pPlayer->getHeight() / 2;
				break;
			case 2:
				fireball->setDirection(Sprite::right);
				fireball->getTransform()->position.x = m_pPlayer->getTransform()->position.x + (float)m_pPlayer->getWidth();
				fireball->getTransform()->position.y = m_pPlayer->getTransform()->position.y + (float)m_pPlayer->getHeight() / 2;
				break;
			case 3:
				fireball->setDirection(Sprite::up);
				fireball->getTransform()->position.x = m_pPlayer->getTransform()->position.x + (float)m_pPlayer->getWidth() / 2;
				fireball->getTransform()->position.y = m_pPlayer->getTransform()->position.y;
				break;
			case 4:
				fireball->setDirection(Sprite::down);
				fireball->getTransform()->position.x = m_pPlayer->getTransform()->position.x + (float)m_pPlayer->getWidth() / 2;
				fireball->getTransform()->position.y = m_pPlayer->getTransform()->position.y + (float)m_pPlayer->getHeight();
				break;
			default:
				break;
			}
		}
	}
	if (!EventManager::Instance().getMouseButton(2))
	{
		m_pRightButtonPressed = false;
	}
	
	/*if(!m_pLeftButtonPressed)
	{
		if (EventManager::Instance().getMouseButton(0))
		{
			m_pLeftButtonPressed = true;
			SDL_Rect temp;

			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_HIT_RIGHT);
				std::cout << "hit right!" << std::endl;
				temp.x = m_pPlayer->getTransform()->position.x + m_pPlayer->getWidth();
				SoundManager::Instance().playSound("melee", 0, -1);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_HIT_LEFT);
				std::cout << "hit left!" << std::endl;
				temp.x = m_pPlayer->getTransform()->position.x - m_pPlayer->getWidth();
				SoundManager::Instance().playSound("melee", 0, -1);
			}

			temp.y = m_pPlayer->getTransform()->position.y;
			temp.w = m_pPlayer->getWidth();
			temp.h = m_pPlayer->getHeight();

			for (auto m_pEnemy : m_enemyVec)
			{
				if (CollisionManager::AABBCheck(temp, m_pEnemy))
					m_pEnemy->DecHP(m_pPlayer->getDamage());
			}
		}
	}
	if (!EventManager::Instance().getMouseButton(0))
	{
		m_pLeftButtonPressed = false;
	}		*/

	if(!m_pHPressed)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
		{
			m_isDebugMode=!m_isDebugMode;
			m_pHPressed = true;
			if(m_isDebugMode)
			{
				std::cout << "Enter Debug Mode" << std::endl;
			}
			else
			{
				std::cout << "Quit Debug Mode" << std::endl;
			}
		}
	}
	
	if(EventManager::Instance().isKeyUp(SDL_SCANCODE_H))
	{
		m_pHPressed = false;
	}

	if (!m_pPPressed)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P))
		{
			m_pPPressed = true;
			for (auto enemy : m_enemyVec)
			{
				enemy->setPatrolMode(!enemy->getPatrolMode());
			}
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_P))
	{
		m_pPPressed = false;		
	}

	if (!m_pKPressed)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
		{
			m_pKPressed = true;
			for (auto m_pEnemy : m_enemyVec)
			{
				m_pEnemy->DecHP(m_pPlayer->getDamage());
			}
			SoundManager::Instance().playSound("WEEOOW", 0, -1);
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_K))
	{
		m_pKPressed = false;		
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::buildGrid()
{
	for(int row=0;row<Config::ROW_NUM;++row)
	{
		for(int col=0;col<Config::COL_NUM;++col)
		{
			PathNode* tempNode = new PathNode();
			tempNode->getTransform()->position = glm::vec2(tempNode->getWidth() * col+0.5f*Config::TILE_SIZE, tempNode->getHeight() * row+0.5f * Config::TILE_SIZE);
			m_pathNodeVec.push_back(tempNode);
		}
	}
}

void PlayScene::displayGrid()
{
	for(int i=0;i<(int)m_pathNodeVec.size();i++)
	{
		auto GridColor = (m_pathNodeVec[i]->getLOS()) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		//std::cout << "Path: " << m_pathNodeVec.size() << " Num: " << row * Config::COL_NUM + col << std::endl;
		//std::cout << "Grid: " << m_pathNodeVec[i]->getTransform()->position.x - m_pathNodeVec[i]->getWidth() * 0.5f << " " << m_pathNodeVec[i]->getTransform()->position.y - m_pathNodeVec[i]->getHeight() * 0.5f << std::endl;
		Util::DrawRect(m_pathNodeVec[i]->getTransform()->position - glm::vec2(m_pathNodeVec[i]->getWidth() * 0.5f, m_pathNodeVec[i]->getHeight() * 0.5f), 40, 40);
		Util::DrawRect(m_pathNodeVec[i]->getTransform()->position , 5, 5, GridColor);
	}
}

void PlayScene::LoadMap()
{
	std::ifstream inFile("../Assets/data/level.txt");
	if (inFile.is_open())
	{ // Build the level from Tile prototypes.
		char key;
		for (int row = 0; row < Config::ROW_NUM; row++)
		{
			for (int col = 0; col < Config::COL_NUM; col++)
			{
				inFile >> key;
				switch (key)
				{
				case 'g':
					{
					m_level[row][col] = new Grass(40.0f * col , 40.0f * row ); 
					//std::cout << "Grass: " << m_level[row][col]->getTransform()->position.x << " " << m_level[row][col]->getTransform()->position.y << std::endl;
					break;
					}
				case 'b':
					{
					m_level[row][col] = new Brick(40.0f * col , 40.0f * row ); 
					m_obstacleVec.push_back(m_level[row][col]);
					//std::cout << "DestructibleObstacle Size: " << m_obstacleVec.size() << std::endl;
					//std::cout << "Brick: " << m_level[row][col]->getTransform()->position.x << " " << m_level[row][col]->getTransform()->position.y << std::endl;
					break;
					}
					default:break;
				}
				addChild(m_level[row][col]);
				//std::cout << "Tile: " << m_level[row][col]->getTransform()->position.x << " " << m_level[row][col]->getTransform()->position.y << std::endl;
				// Construct the Node for a valid tile.
				if (!m_level[row][col]->IsObstacle() && !m_level[row][col]->IsHazard())
				{
					m_level[row][col]->m_node = new PathNode();
					m_level[row][col]->m_node->getTransform()->position.x = m_level[row][col]->getTransform()->position.x + 0.5f * Config::TILE_SIZE;
					m_level[row][col]->m_node->getTransform()->position.y = m_level[row][col]->getTransform()->position.y + 0.5f * Config::TILE_SIZE;
					m_pathNodeVec.push_back(m_level[row][col]->m_node);
					m_pathNodeNum++;
					//std::cout << "PathNode[" << row << "][" << col << "] created." << std::endl;
				}
			}
		}
		//std::cout << "DestructibleObstacle Size: " << m_obstacleVec.size() << std::endl;
	}
}

void PlayScene::AddConnection()
{
	for (int row = 0; row < Config::ROW_NUM; row++)
	{
		for (int col = 0; col < Config::COL_NUM; col++)
		{
			if (m_level[row][col]->Node() == nullptr) // Now we can test for nullptr.
				continue; // An obstacle or hazard tile has no connections.
			// Make valid connections. Inside map and a valid tile.
			if (row - 1 != -1 && m_level[row - 1][col]->Node() != nullptr) // Tile above. 
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row - 1][col]->Node(),
					Util::distance( m_level[row][col]->Node()->getTransform()->position, m_level[row-1][col]->Node()->getTransform()->position)));
			if (row + 1 != Config::ROW_NUM && m_level[row + 1][col]->Node() != nullptr) // Tile below.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row + 1][col]->Node(),
					Util::distance(m_level[row][col]->Node()->getTransform()->position, m_level[row + 1][col]->Node()->getTransform()->position)));
			if (col - 1 != -1 && m_level[row][col - 1]->Node() != nullptr) // Tile to Left.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row][col - 1]->Node(),
					Util::distance(m_level[row][col]->Node()->getTransform()->position, m_level[row][col - 1]->Node()->getTransform()->position)));
			if (col + 1 != Config::COL_NUM && m_level[row][col + 1]->Node() != nullptr) // Tile to right.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row][col + 1]->Node(),
					Util::distance(m_level[row][col]->Node()->getTransform()->position, m_level[row][col + 1]->Node()->getTransform()->position)));
		}
	}
}

void PlayScene::setGridLOS()
{
	for(auto m_pNode:m_pathNodeVec)
	{
		for(auto obstacle:m_obstacleVec)
		{
			m_pNode->setLOS(CollisionManager::LOSCheck(m_pNode, m_pPlayer, obstacle));
			if (m_pNode->getLOS())
			{
				break;
			}
		}
	}
}

void PlayScene::QueueLine(const glm::vec2 start, const glm::vec2 end, const glm::vec4 col)
{
	m_pointVec.push_back(start);
	m_pointVec.push_back(end);
	m_colorVec.push_back(col);
}

void PlayScene::DrawLine()
{
	for (unsigned i = 0, j = 0; i < m_pointVec.size(); i += 2, j++)
	{
		QueueLine(m_pointVec[i], m_pointVec[i + 1], m_colorVec[j]);
	}
	m_pointVec.clear();
	m_colorVec.clear();
	m_pointVec.shrink_to_fit();
	m_colorVec.shrink_to_fit();
}

void PlayScene::DrawPath()
{
	for (unsigned i = 0; i < PathManager::getPath().size(); i++)
	{
		QueueLine({ PathManager::getPath()[i]->GetFromNode()->getTransform()->position.x + 16, PathManager::getPath()[i]->GetFromNode()->getTransform()->position.y + 16 },
			{ Util::HalfwayPoint(PathManager::getPath()[i]->GetFromNode()->Pt(), PathManager::getPath()[i]->GetToNode()->Pt()).x + 16,
			Util::HalfwayPoint(PathManager::getPath()[i]->GetFromNode()->Pt(), PathManager::getPath()[i]->GetToNode()->Pt()).y + 16 }, { 255,255,0,255 });
		QueueLine({ Util::HalfwayPoint(PathManager::getPath()[i]->GetFromNode()->Pt(), PathManager::getPath()[i]->GetToNode()->Pt()).x + 16,
			Util::HalfwayPoint(PathManager::getPath()[i]->GetFromNode()->Pt(), PathManager::getPath()[i]->GetToNode()->Pt()).y + 16 },
			{ PathManager::getPath()[i]->GetToNode()->getTransform()->position.x + 16, PathManager::getPath()[i]->GetToNode()->getTransform()->position.y + 16 }, { 255,128,0,255 });
	}
}

PathNode* PlayScene::getPathNode(int x, int y)
{
	int Index = (x - 1) * Config::ROW_NUM + (y - 1) * Config::COL_NUM;
	return m_pathNodeVec[Index];
}

void PlayScene::drawLOS()
{	
	for (auto node : m_pathNodeVec)
	{
		if(!node->getLOS())
		{
			Util::DrawLine(m_pPlayer->getTransform()->position, node->getTransform()->position, glm::vec4(1, 0, 0, 0));
		}
		else
		{
			Util::DrawLine(m_pPlayer->getTransform()->position, node->getTransform()->position,glm::vec4(0,0,1,0));
		}			
	}
}

void PlayScene::RemoveNullObject()
{
	/*if (!m_pFireballVec.empty())
	{
		m_pFireballVec.erase(remove(m_pFireballVec.begin(), m_pFireballVec.end(), nullptr), m_pFireballVec.end());
	}*/
	if (!m_enemyVec.empty())
	{
		m_enemyVec.erase(remove(m_enemyVec.begin(), m_enemyVec.end(), nullptr), m_enemyVec.end());
	}
	if (!m_obstacleVec.empty())
	{
		m_obstacleVec.erase(remove(m_obstacleVec.begin(), m_obstacleVec.end(), nullptr), m_obstacleVec.end());
	}
	removeNullPointer();
}


void PlayScene::start()
{
	m_isDebugMode = false;
	m_pHPressed = false;
	m_pKPressed = false;
	m_pPPressed = false;
	m_pathNodeNum = 0;
	
	//buildGrid();
	LoadMap();
	AddConnection();
	// Plane Sprite
	//m_enemyVec.push_back(new Plane(400.0f, 100.0f));
	//for (auto m_pEnemy : m_enemyVec)
	//{
	//	addChild(m_pEnemy);
	//	//std::cout << "Position: " << m_pEnemy->getTransform()->position.x << " " << m_pEnemy->getTransform()->position.y << std::endl;
	//}
	//std::cout << "Enemy: " << (int)m_enemyVec.size() << " "<< numberOfChildren()<<std::endl;
	/*m_enemyVec[0]->setStartNode(getPathNode(10, 3));
	m_enemyVec[0]->setEndNode(getPathNode(10, 7));*/

	/*m_enemyVec[0]->addPathNode(m_pathNodeVec[0]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[1]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[2]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[3]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[4]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[5]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[6]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[7]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[8]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[9]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[10]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[11]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[12]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[13]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[14]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[15]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[16]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[17]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[18]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[19]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[39]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[59]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[79]);	
	m_enemyVec[0]->addPathNode(m_pathNodeVec[78]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[77]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[76]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[75]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[74]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[73]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[72]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[71]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[70]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[69]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[68]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[67]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[66]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[65]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[64]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[63]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[62]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[61]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[60]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[40]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[20]);*/

	/*m_enemyVec[0]->AddKeyNode(m_pathNodeVec[0]);
	m_enemyVec[0]->AddKeyNode(m_pathNodeVec[19]);
	m_enemyVec[0]->AddKeyNode(m_pathNodeVec[79]);
	m_enemyVec[0]->AddKeyNode(m_pathNodeVec[60]);*/
	//m_enemyVec[0]->setPath();

	for(auto enemy:m_enemyVec)
	{
		//enemy->getTransform()->position =  enemy->getPatrolPath()[0]->getTransform()->position;
		enemy->getTransform()->position = enemy->getKeyNode()[0]->m_keyNode->getTransform()->position;
		enemy->getRigidBody()->maxSpeed = 5.0f;
		enemy->setCurTargetKdyNode(enemy->getKeyNode()[1]);
		//std::cout << "To: " << enemy->getCurTargetKeyNode()->m_keyNode->getTransform()->position.x << " " << enemy->getCurTargetKeyNode()->m_keyNode->getTransform()->position.y << std::endl;
		/*for(int i=0;i<(int)enemy->getKeyNode().size();i++)
		{
			std::cout << "Node " << i + 1 << ": " << enemy->getKeyNode()[i]->m_keyNode->getTransform()->position.x << " " << enemy->getKeyNode()[i]->m_keyNode->getTransform()->position.y << std::endl;
		}*/
	}
	
	auto warrior = EnemyManager::Instance()->generateWarrior();
	warrior->getTransform()->position.x = 200;
	warrior->getTransform()->position.y = 200;
	addChild(warrior);
	
	auto archer = EnemyManager::Instance()->generateArcher();
	archer->getTransform()->position.x = 300;
	archer->getTransform()->position.y = 300;
	addChild(archer);
	
	// Player Sprite
	m_pPlayer = new Player(100.0f,500.0f);
	addChild(m_pPlayer);
	//m_playerFacingRight = true;

	//Load Sound
	SoundManager::Instance().load("../Assets/audio/Battle_BGM.wav", "BGM", SOUND_MUSIC);
	SoundManager::Instance().load("../Assets/audio/Footsteps.wav", "step", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/engine.wav", "engine", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/WEEEOOOOWW.wav", "WEEOOW", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Sword_hit.wav", "melee", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Fireball_shot.wav", "fireball", SOUND_SFX);

	SoundManager::Instance().playMusic("BGM", -1, 0);
	SoundManager::Instance().setMusicVolume(50);
	SoundManager::Instance().setSoundVolume(50);

	const SDL_Color blue = { 0, 0, 255, 255 };
	m_pScoreLabel = new Label("SCORE: ", "Consolas", 40, blue, glm::vec2(400.0f, 40.0f));
	m_pScoreLabel->setParent(this);
	addChild(m_pScoreLabel);

	m_pEnemyKilledLabel = new Label("0", "Consolas", 40, blue, glm::vec2(470.0f, 40.0f));
	m_pEnemyKilledLabel->setParent(this);
	addChild(m_pEnemyKilledLabel);
}
