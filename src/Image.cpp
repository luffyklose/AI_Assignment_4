#include "Image.h"
#include "TextureManager.h"

Image::Image(float x, float y)
{
	TextureManager::Instance()->load("../Assets/textures/Logo.png", "logo");
	auto size = TextureManager::Instance()->getTextureSize("logo");
	setWidth(size.x / 2);
	setHeight(size.y / 2);
	
	getTransform()->position = glm::vec2(x, y);
	
}

Image::~Image()
= default;

void Image::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	
	TextureManager::Instance()->draw("logo", x, y, this->getWidth(), this->getHeight(), 0, 255, true);
}

void Image::update()
{
}

void Image::clean()
{
}
