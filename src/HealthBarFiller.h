#pragma once
#include "../src/Sprite.h"
#include "UIControl.h"

class HealthBarFiller: public UIControl
{
public:
	HealthBarFiller(Sprite* sp);
	~HealthBarFiller();

	void draw() override;
	void update() override;
	void clean() override;

private:
	Sprite* m_pSp;
};
