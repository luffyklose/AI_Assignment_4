#pragma once
#include "../src/Sprite.h"
#include "UIControl.h"

class Image : public UIControl
{
public:
	Image(float x, float y);
	~Image();
	
	void draw() override;
	void update() override;
	void clean() override;
};

