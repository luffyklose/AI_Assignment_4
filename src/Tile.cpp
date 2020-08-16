#include "Tile.h"

#include "TextureManager.h"

Tile::Tile( float x,float y)
{	
	setWidth(40);
	setHeight(40);
	
	getTransform()->position = glm::vec2(x,y);
	getRigidBody()->isColliding = false;
	m_node = nullptr;
	setType(TILE);
}

Tile::~Tile()
{
	delete m_node; // Will also invoke PathNode destructor. Deletes connections.
}


Grass::Grass(float x, float y):Tile(x,y)
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

Brick::Brick(float x, float y) :Tile(x, y)
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

BarrelTile::BarrelTile(float x, float y):Tile(x,y)
{

	m_pGrass=new Grass(x,y);
	//m_pObstacle = new DestructibleObstacle(x, y,64,64);	

	m_obstacle = true;
	m_hazard = false;
	m_hasObstacle = true;
}

void BarrelTile::draw()
{
	m_pGrass->draw();
	m_pObstacle->draw();
}

void BarrelTile::update()
{
	m_pGrass->update();
	m_pObstacle->update();
}

void BarrelTile::clean()
{
}
