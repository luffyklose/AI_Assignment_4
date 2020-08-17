#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"
#include"PathManager.h"
#include <fstream>

#include "DestructibleObstacleManager.h"
#include "ProjectileManager.h"
#include"SoundManager.h"
#include "EnemyManager.h"
#include "ExplosionManager.h"
#include "NodeManager.h"
#include "PortalManager.h"

//std::vector<PathNode*> PlayScene::m_pathNodeVec;

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
		Util::DrawRect(m_pPlayer->getTransform()->position - glm::vec2(0.5f * m_pPlayer->getWidth(), 0.5f * m_pPlayer->getHeight()), m_pPlayer->getWidth(), m_pPlayer->getHeight());

		for (auto m_pEnemy :EnemyManager::Instance()->getEnemyVec())
		{
			auto LOSColour = (m_pEnemy->getHasLOS()) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			auto CircleColour = (m_pEnemy->getDetect()) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			Util::DrawLine(m_pPlayer->getTransform()->position, m_pEnemy->getTransform()->position, LOSColour);
			Util::DrawRect(m_pEnemy->getTransform()->position - glm::vec2(0.5f * m_pEnemy->getWidth(), 0.5f * m_pEnemy->getHeight()), m_pEnemy->getWidth(), m_pEnemy->getHeight());
			Util::DrawCircle(m_pEnemy->getTransform()->position, m_pEnemy->getDetectionRadius(), CircleColour);
		}

		/*for(auto m_pStean:m_obstacleVec)
		{
			Util::DrawRect(m_pStean->getTransform()->position - glm::vec2(m_pStean->getWidth() * 0.5f, m_pStean->getHeight() * 0.5f), m_pStean->getWidth(), m_pStean->getHeight());

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

	//std::cout << "Player fireball: " << ProjectileManager::Instance()->getPlayerFireVec().size() << std::endl;
	//std::cout << "Enemy fireball: " << ProjectileManager::Instance()->getEnemyFireVec().size() << std::endl;
	for (auto fireball : ProjectileManager::Instance()->getPlayerFireVec())
	{
		for (auto enemy : EnemyManager::Instance()->getEnemyVec())
		{
			if (CollisionManager::AABBCheck(fireball, enemy))
			{
				std::cout << "Player hit enemy" << std::endl;
				enemy->DecHP(m_pPlayer->getRangeDamage());
				fireball->setIsActive(false);
			}
		}

		for (auto obstacle : m_obstacleVec)
		{
			if (CollisionManager::AABBCheck(fireball, obstacle))
			{
				fireball->setIsActive(false);
			}
		}

		for(auto stean:DestructibleObstacleManager::Instance()->getSteanVec())
		{
			if(CollisionManager::AABBCheck(fireball,stean))
			{
				int row = stean->getRow();
				int col = stean->getCol();
				stean->DecHP(m_pPlayer->getRangeDamage());
				//std::cout << "Stean HP: " << stean->getCurHealth() << std::endl;
				if(!stean->isActive())
				{
					AddPathNode(m_level[row][col]);
					AddSingleNodeConnection(row, col);
				}
				//std::cout << "Stean " << stean->isActive() << std::endl;
				fireball->setIsActive(false);
			}
		}

		if (fireball->getTransform()->position.x<-0.5f * fireball->getWidth() ||
			fireball->getTransform()->position.x > Config::SCREEN_WIDTH + 0.5f * fireball->getWidth() ||
			fireball->getTransform()->position.y < -0.5f * fireball->getHeight() ||
			fireball->getTransform()->position.y > Config::SCREEN_HEIGHT + 0.5f * fireball->getHeight())
		{
			fireball->setIsActive(false);
		}
	}

	for (auto fireball : ProjectileManager::Instance()->getEnemyFireVec())
	{
		if (CollisionManager::AABBCheck(fireball, m_pPlayer))
		{
			m_pPlayer->DecHP(20);
			fireball->setIsActive(false);
		}

		for (auto obstacle : m_obstacleVec)
		{
			if (CollisionManager::AABBCheck(fireball, obstacle))
			{
				fireball->setIsActive(false);
			}
		}

		for (auto stean : DestructibleObstacleManager::Instance()->getSteanVec())
		{
			if (CollisionManager::AABBCheck(fireball, stean))
			{
				stean->DecHP(20);
				fireball->setIsActive(false);
			}
		}

		if (fireball->getTransform()->position.x < -0.5f * fireball->getWidth() ||
			fireball->getTransform()->position.x > Config::SCREEN_WIDTH + 0.5f * fireball->getWidth() ||
			fireball->getTransform()->position.y < -0.5f * fireball->getHeight() ||
			fireball->getTransform()->position.y > Config::SCREEN_HEIGHT + 0.5f * fireball->getHeight())
		{
			fireball->setIsActive(false);
		}
	}

	
	for(auto m_pEnemy : EnemyManager::Instance()->getEnemyVec())
	{
		//m_pEnemy->detectPlayer(m_pPlayer);
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
	
	//RemoveNullObject();
	EnemyManager::Instance()->RemoveInvalid();
	ProjectileManager::Instance()->RemoveInvalid();
	DestructibleObstacleManager::Instance()->RemoveInvalid();
}

void PlayScene::clean()
{
	if (!m_obstacleVec.empty())
	{
		m_obstacleVec.erase(remove(m_obstacleVec.begin(), m_obstacleVec.end(), nullptr), m_obstacleVec.end());
	}
	EnemyManager::Instance()->exit();
	ProjectileManager::Instance()->exit();
	DestructibleObstacleManager::Instance()->exit();
	removeNullPointer();
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
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A) &&
			!CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(-5.0f, 0.0f), m_obstacleVec) &&
			!CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(-5.0f, 0.0f), DestructibleObstacleManager::Instance()->getSteanVec()) &&
			m_pPlayer->getTransform()->position.x > 0.5f * m_pPlayer->getWidth())
		{
			m_pPlayer->setAnimationState(PLAYER_WALK_LEFT);
			m_pPlayer->setDirection(Sprite::left);

			m_pPlayer->getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("step", 0, -1);
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D) && 
			!CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(5.0f, 0.0f), m_obstacleVec) &&
			!CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(5.0f, 0.0f), DestructibleObstacleManager::Instance()->getSteanVec()) &&
			m_pPlayer->getTransform()->position.x < Config::SCREEN_WIDTH - 0.5f * m_pPlayer->getWidth())
		{
			m_pPlayer->setAnimationState(PLAYER_WALK_RIGHT);
			m_pPlayer->setDirection(Sprite::right);

			m_pPlayer->getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("step", 0, -1);
		}
		else if(EventManager::Instance().isKeyDown(SDL_SCANCODE_W) && 
			!CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(0.0f, -5.0f), m_obstacleVec) &&
			!CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(0.0f, -5.0f), DestructibleObstacleManager::Instance()->getSteanVec()) &&
			m_pPlayer->getTransform()->position.y > 0.5f * m_pPlayer->getHeight())
		{
			m_pPlayer->setAnimationState(PLAYER_WALK_UP);
			m_pPlayer->setDirection(Sprite::up);
			
			m_pPlayer->getRigidBody()->velocity = glm::vec2(0.0f, -5.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("step", 0, -1);
		}
		else if(EventManager::Instance().isKeyDown(SDL_SCANCODE_S) && 
			!CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(0.0f, 5.0f), m_obstacleVec) &&
			!CollisionManager::PlayerCollision(m_pPlayer, glm::vec2(0.0f, 5.0f), DestructibleObstacleManager::Instance()->getSteanVec()) &&
			m_pPlayer->getTransform()->position.y < Config::SCREEN_HEIGHT - 0.5f * m_pPlayer->getHeight())
		{
			m_pPlayer->setAnimationState(PLAYER_WALK_DOWN);
			m_pPlayer->setDirection(Sprite::down);
			
			m_pPlayer->getRigidBody()->velocity = glm::vec2(0.0f, 5.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("step", 0, -1);
		}
		else
		{
			if (m_pPlayer->getDirection()==Sprite::right || m_pPlayer->getDirection() == Sprite::down)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else if(m_pPlayer->getDirection() == Sprite::left || m_pPlayer->getDirection() == Sprite::up)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
		
		/*switch (m_pPlayer->getDirection())
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
		}*/
	}

	if(!m_pRightButtonPressed)
	{
		if(EventManager::Instance().getMouseButton(2))
		{
			m_pRightButtonPressed = true;

			ProjectileManager::Instance()->generateFireball();
			FireBall* fireball = ProjectileManager::Instance()->getFireBallList().back();
			//ProjectileManager::Instance()->getPlayerFireVec().push_back(fireball);
			ProjectileManager::Instance()->addFireBall2PlayerVec(fireball);
			std::cout << "Player fireball: " << ProjectileManager::Instance()->getPlayerFireVec().size() << std::endl;
			
			switch (m_pPlayer->getDirection())
			{
			case 1:
				fireball->setDirection(Sprite::left);
				fireball->getRigidBody()->velocity = glm::vec2(-fireball->getSpeed(),0);				
				fireball->getTransform()->position.x = m_pPlayer->getTransform()->position.x - 0.5f * m_pPlayer->getWidth() - 0.5f * fireball->getWidth();
				fireball->getTransform()->position.y = m_pPlayer->getTransform()->position.y;
				break;
			case 2:
				fireball->setDirection(Sprite::right);
				fireball->getRigidBody()->velocity = glm::vec2(fireball->getSpeed(), 0);
				fireball->getTransform()->position.x = m_pPlayer->getTransform()->position.x + 0.5f * m_pPlayer->getWidth() + 0.5f * fireball->getWidth();
				fireball->getTransform()->position.y = m_pPlayer->getTransform()->position.y;
				break;
			case 3:
				fireball->setDirection(Sprite::up);
				fireball->getRigidBody()->velocity = glm::vec2(0, -fireball->getSpeed());
				fireball->getTransform()->position.x = m_pPlayer->getTransform()->position.x;
				fireball->getTransform()->position.y = m_pPlayer->getTransform()->position.y - 0.5f * m_pPlayer->getHeight() - 0.5f * fireball->getHeight();
				break;
			case 4:
				fireball->setDirection(Sprite::down);
				fireball->getRigidBody()->velocity = glm::vec2(0, fireball->getSpeed());
				fireball->getTransform()->position.x = m_pPlayer->getTransform()->position.x;
				fireball->getTransform()->position.y = m_pPlayer->getTransform()->position.y + 0.5f * m_pPlayer->getHeight() + 0.5f * fireball->getHeight();
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

	/*if (!m_pPPressed)
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
	}*/

	if (!m_pKPressed)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
		{
			m_pKPressed = true;
			for (auto m_pEnemy : EnemyManager::Instance()->getEnemyVec())
			{
				m_pEnemy->DecHP(m_pPlayer->getMeleeDamage());
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
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* tempNode = new PathNode();
			tempNode->getTransform()->position = glm::vec2(tempNode->getWidth() * col + 0.5f * Config::TILE_SIZE, tempNode->getHeight() * row + 0.5f * Config::TILE_SIZE);
			NDMA::AddPathNode(tempNode);
		}
	}
}

void PlayScene::displayGrid()
{
	for (int i = 0; i < (int)NDMA::getPathNodeVec().size(); i++)
	{
		auto GridColor = (NDMA::getPathNodeVec()[i]->getLOS()) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		//std::cout << "Path: " << m_pathNodeVec.size() << " Num: " << row * Config::COL_NUM + col << std::endl;
		//std::cout << "Grid: " << m_pathNodeVec[i]->getTransform()->position.x - m_pathNodeVec[i]->getWidth() * 0.5f << " " << m_pathNodeVec[i]->getTransform()->position.y - m_pathNodeVec[i]->getHeight() * 0.5f << std::endl;
		Util::DrawRect(NDMA::getPathNodeVec()[i]->getTransform()->position - glm::vec2(NDMA::getPathNodeVec()[i]->getWidth() * 0.5f, NDMA::getPathNodeVec()[i]->getHeight() * 0.5f), 40, 40);
		Util::DrawRect(NDMA::getPathNodeVec()[i]->getTransform()->position, 5, 5, GridColor);
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
					m_level[row][col] = new Grass(40.0f * col + 0.5 * Config::TILE_SIZE, 40.0f * row + 0.5 * Config::TILE_SIZE,row,col);
					//std::cout << "Grass: " << m_level[row][col]->getTransform()->position.x << " " << m_level[row][col]->getTransform()->position.y << std::endl;
					break;
					}
				case 'b':
					{
					m_level[row][col] = new Brick(40.0f * col + 0.5 * Config::TILE_SIZE, 40.0f * row + 0.5 * Config::TILE_SIZE, row, col);
					m_obstacleVec.push_back(m_level[row][col]);
					//std::cout << "DestructibleObstacle Size: " << m_obstacleVec.size() << std::endl;
					//std::cout << "Brick: " << m_level[row][col]->getTransform()->position.x << " " << m_level[row][col]->getTransform()->position.y << std::endl;
					break;
					}
				case 's':
					{
					m_level[row][col] = new SteanTile(40.0f * col + 0.5 * Config::TILE_SIZE, 40.0f * row + 0.5 * Config::TILE_SIZE, row, col);
					break;
					}
					default:break;
				}
				addChild(m_level[row][col]);
				//std::cout << "Tile: " << m_level[row][col]->getTransform()->position.x << " " << m_level[row][col]->getTransform()->position.y << std::endl;
				// Construct the Node for a valid tile.
				if (!m_level[row][col]->IsObstacle() && !m_level[row][col]->IsHazard() && !m_level[row][col]->HasObstacle())
				{
					m_level[row][col]->m_node = new PathNode();
					m_level[row][col]->m_node->getTransform()->position.x = m_level[row][col]->getTransform()->position.x;
					m_level[row][col]->m_node->getTransform()->position.y = m_level[row][col]->getTransform()->position.y;
					NDMA::AddPathNode(m_level[row][col]->m_node);
					m_pathNodeNum++;
					//std::cout << "PathNode[" << row << "][" << col << "] created." << std::endl;
				}
			}
		}
		//std::cout << "DestructibleObstacle Size: " << m_obstacleVec.size() << std::endl;
	}
}

void PlayScene::AddPathNode(Tile* tile)
{
	tile->m_node = new PathNode();
	tile->m_node->getTransform()->position.x = tile->getTransform()->position.x;
	tile->m_node->getTransform()->position.y = tile->getTransform()->position.y;
	NDMA::AddPathNode(tile->m_node);
	m_pathNodeNum++;
}

void PlayScene::AddSingleNodeConnection(int row,int col)
{
	if (m_level[row][col]->Node() == nullptr) // Now we can test for nullptr.
		return; // An obstacle or hazard tile has no connections.
	// Make valid connections. Inside map and a valid tile.
	if (row - 1 != -1 && m_level[row - 1][col]->Node() != nullptr) // Tile above. 
		m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row - 1][col]->Node(),
			Util::distance(m_level[row][col]->Node()->getTransform()->position, m_level[row - 1][col]->Node()->getTransform()->position)));
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

void PlayScene::AddConnection()
{
	for (int row = 0; row < Config::ROW_NUM; row++)
	{
		for (int col = 0; col < Config::COL_NUM; col++)
		{
			AddSingleNodeConnection(row,col);
			//if (m_level[row][col]->Node() == nullptr) // Now we can test for nullptr.
			//	continue; // An obstacle or hazard tile has no connections.
			//// Make valid connections. Inside map and a valid tile.
			//if (row - 1 != -1 && m_level[row - 1][col]->Node() != nullptr) // Tile above. 
			//	m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row - 1][col]->Node(),
			//		Util::distance(m_level[row][col]->Node()->getTransform()->position, m_level[row - 1][col]->Node()->getTransform()->position)));
			//if (row + 1 != Config::ROW_NUM && m_level[row + 1][col]->Node() != nullptr) // Tile below.
			//	m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row + 1][col]->Node(),
			//		Util::distance(m_level[row][col]->Node()->getTransform()->position, m_level[row + 1][col]->Node()->getTransform()->position)));
			//if (col - 1 != -1 && m_level[row][col - 1]->Node() != nullptr) // Tile to Left.
			//	m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row][col - 1]->Node(),
			//		Util::distance(m_level[row][col]->Node()->getTransform()->position, m_level[row][col - 1]->Node()->getTransform()->position)));
			//if (col + 1 != Config::COL_NUM && m_level[row][col + 1]->Node() != nullptr) // Tile to right.
			//	m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row][col + 1]->Node(),
			//		Util::distance(m_level[row][col]->Node()->getTransform()->position, m_level[row][col + 1]->Node()->getTransform()->position)));
		}
	}
}



void PlayScene::setGridLOS()
{
	for(auto m_pNode: NDMA::getPathNodeVec())
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
	return NDMA::getPathNodeVec()[Index];
}

void PlayScene::drawLOS()
{	
	for (auto node : NDMA::getPathNodeVec())
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

void PlayScene::start()
{
	m_isDebugMode = false;
	m_pHPressed = false;
	m_pKPressed = false;
	m_pPPressed = false;
	m_pathNodeNum = 0;

	//set stean
	DestructibleObstacleManager::Instance()->Init();
	for (auto stean : DestructibleObstacleManager::Instance()->getDesObsList())
	{
		addChild(stean);
	}
	
	//buildGrid();
	LoadMap();
	AddConnection();
	NDMA::AddFleeNode(NDMA::getPathNodeVec()[0]);

	//set portal
	PortalManager::Instance()->CreateCreatePortal(NDMA::getPathNodeVec()[20]->getTransform()->position);
	PortalManager::Instance()->CreateCreatePortal(NDMA::getPathNodeVec()[50]->getTransform()->position);
	PortalManager::Instance()->CreateFleePortal(NDMA::getPathNodeVec()[0]->getTransform()->position);
	for (auto portal : PortalManager::Instance()->getCreatePortalVec())
	{
		addChild(portal);
	}
	for (auto portal : PortalManager::Instance()->getFleePortalVec())
	{
		addChild(portal);
	}
	
	// Set Player
	m_pPlayer = new Player(100.0f, 500.0f);
	addChild(m_pPlayer);	

	//Set Enemies
	EnemyManager::Instance()->setTarget(m_pPlayer);
	EnemyManager::Instance()->Init();	
	for(auto warrior:EnemyManager::Instance()->getWarriorList())
	{
		addChild(warrior);
	}
	for(auto archer:EnemyManager::Instance()->getArcherList())
	{
		addChild(archer);
	}
	
	//EnemyManager::Instance()->generateWarrior();
	
	//EnemyManager::Instance()->generateArcher();

	//Set Fireball
	ProjectileManager::Instance()->Init();
	for(auto fireball:ProjectileManager::Instance()->getFireBallList())
	{
		addChild(fireball);
	}

	//set explosion
	ExplosionManager::Instance()->Init();
	for(auto explosion:ExplosionManager::Instance()->getExplosionList())
	{
		addChild(explosion);
	}

	//Set destructible Obstacle
	DestructibleObstacleManager::Instance()->Init();
	for(auto obstacle:DestructibleObstacleManager::Instance()->getDesObsList())
	{
		addChild(obstacle);
	}
	
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
