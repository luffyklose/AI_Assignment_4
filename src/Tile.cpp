#include "Tile.h"


#include "DestructibleObstacleManager.h"
#include "TextureManager.h"

Tile::Tile( float x,float y, int row, int col)
{	
	setWidth(40);
	setHeight(40);

	m_row = row;
	m_col = col;
	getTransform()->position = glm::vec2(x,y);
	getRigidBody()->isColliding = false;
	m_node = nullptr;
	setType(TILE);
}

Tile::~Tile()
{
	delete m_node; // Will also invoke PathNode destructor. Deletes connections.
}


Grass::Grass(float x, float y, int row, int col):Tile(x,y,row,col)
{
	TextureManager::Instance()->load("../Assets/textures/grass.png", "grass");

	auto size = TextureManager::Instance()->getTextureSize("grass");

	m_obstacle = false;
	m_hazard = false;
	m_hasObstacle = false;
}

void Grass::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance()->draw("grass", x , y , getWidth(), getHeight(), 0, 255, true);
}

void Grass::update()
{
}

void Grass::clean()
{
}

Brick::Brick(float x, float y, int row, int col) :Tile(x, y,row,col)
{
	TextureManager::Instance()->load("../Assets/textures/brick.png", "brick");

	auto size = TextureManager::Instance()->getTextureSize("brick");

	m_obstacle = true;
	m_hazard = false;
	m_hasObstacle = false;
}

void Brick::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance()->draw("brick", x , y , getWidth(), getHeight(), 0, 255, true);
}

void Brick::update()
{
}

void Brick::clean()
{
}

SteanTile::SteanTile(float x, float y, int row, int col):Tile(x,y,row,col)
{

	m_pGrass=new Grass(x,y,row,col);
	//m_pStean = new DestructibleObstacle(x, y,64,64);
	DestructibleObstacleManager::Instance()->generateObstacle();
	m_pStean = DestructibleObstacleManager::Instance()->getDesObsList().back();
	DestructibleObstacleManager::Instance()->AddStean(m_pStean);
	m_pStean->getTransform()->position = glm::vec2(x, y);
	m_pStean->setRow(row);
	m_pStean->setCol(col);

	m_obstacle = false;
	m_hazard = false;
	m_hasObstacle = true;
}

void SteanTile::draw()
{
	m_pGrass->draw();
	m_pStean->draw();
}

void SteanTile::update()
{
	m_pGrass->update();
	m_pStean->update();
}

void SteanTile::clean()
{
}
