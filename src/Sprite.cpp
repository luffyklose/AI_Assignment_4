#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
= default;

SpriteSheet* Sprite::getSpriteSheet()
{
	return m_pSpriteSheet;
}

Animation& Sprite::getAnimation(const std::string& name)
{
	return m_pAnimations[name];
}

void Sprite::setSpriteSheet(SpriteSheet* sprite_sheet)
{
	m_pSpriteSheet = sprite_sheet;
}

void Sprite::setAnimation(const Animation& animation)
{
	if(!m_animationExists(animation.name))
	{
		m_pAnimations[animation.name] = animation;
	}
}

bool Sprite::m_animationExists(const std::string& id)
{
	return m_pAnimations.find(id) != m_pAnimations.end();
}

void Sprite::setDirection(Direction dir)
{
	switch (dir)
	{
	case 1:
		m_dir = left;
		break;
	case 2:
		m_dir = right;
		break;
	case 3:
		m_dir = up;
		break;
	case 4:
		m_dir = down;
		break;
	default:
		break;
	}
}