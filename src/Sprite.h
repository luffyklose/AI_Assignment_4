#pragma once
#ifndef __SPRITE__
#define __SPRITE__

#include "DisplayObject.h"
#include "Animation.h"
#include <unordered_map>
#include "SpriteSheet.h"



class Sprite : public DisplayObject
{
public:
	Sprite();
	virtual ~Sprite();
	
	// Life Cycle Functions
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void clean() = 0;

	// getters
	SpriteSheet* getSpriteSheet();
	Animation& getAnimation(const std::string& name);
	
	// setters
	void setSpriteSheet(SpriteSheet* sprite_sheet);
	void setAnimation(const Animation& animation);
	void setAngle(float angle) { m_angle = angle; }
	virtual int getMaxhealth()=0;
	int getCurHealth() { return m_curHealth; }

	enum Direction
	{
		left = 1,
		right,
		up,
		down
	};

	void setDirection(Direction dir);
	int getDirection() { return m_dir; }
	
protected:
	// private utility functions
	bool m_animationExists(const std::string& id);

	SpriteSheet* m_pSpriteSheet;

	std::unordered_map<std::string, Animation> m_pAnimations;

	float m_angle;
	int m_curHealth;

	Direction m_dir;	
};

#endif /* defined (__SPRITE__) */