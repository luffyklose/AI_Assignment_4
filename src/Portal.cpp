#include "Portal.h"

#include "TextureManager.h"

Portal::Portal()
{
	TextureManager::Instance()->load("../Assets/textures/portal.png", "portal");

	const auto size = TextureManager::Instance()->getTextureSize("portal");
	setWidth(40);
	setHeight(40);

	setType(PORTAL);
	m_curHealth = 100;
}

Portal::~Portal()
{
}

void Portal::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("portal", x, y, getWidth(), getHeight(), 0, 255, true);
}

void Portal::update()
{
}

void Portal::clean()
{
}

int Portal::getMaxhealth()
{
	return 0;
}
