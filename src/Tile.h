#pragma once
#include "DisplayObject.h"
#include "PathNode.h"
#include "Label.h"
#include"DestructibleObstacle.h"

class Tile : public DisplayObject
{
public:
	Tile( float x, float y,int row,int col);
	~Tile();
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void clean() = 0;

	//getter
	bool IsObstacle() { return m_obstacle; }
	bool IsHazard() { return m_hazard; }
	bool HasObstacle() { return m_hasObstacle; }
	int getRow() { return m_row; }
	int getCol() { return m_col; }
	
	PathNode* Node() { return m_node; }
	//virtual int getMaxhealth() = 0;
public:
	PathNode* m_node;
protected:
	bool m_obstacle,
		m_hazard,
		m_hasObstacle;
	int m_row, m_col;
};

class Grass: public Tile
{
public:
	Grass(float x, float y,int row,int col);

	//Grass* Clone() { return new Grass(getTransform()->position.x, getTransform()->position.y); }

	void draw() override;
	void update() override;
	void clean() override;

	//int getMaxhealth() { return grassHP; }
private:
	
public:
	
private:
	//const int grassHP = 0;
};

class Brick : public Tile
{
public:
	Brick(float x, float y, int row, int col);
	//Brick* Clone() { return new Brick(getTransform()->position.x, getTransform()->position.y); }
	
	void draw() override;
	void update() override;
	void clean() override;

	//int getMaxhealth() { return brickHP; }
private:

public:

private:
	//const int brickHP = 50;
};

class SteanTile:public Tile
{
public:
	SteanTile(float x, float y, int row, int col);
	//SteanTile* Clone() { return new SteanTile(getTransform()->position.x, getTransform()->position.y); }

	void draw() override;
	void update() override;
	void clean() override;

	//int getMaxhealth() { return brickHP; }
private:

public:

private:
	//const int brickHP = 50;
	Tile* m_pGrass;
	DestructibleObstacle* m_pStean;
};