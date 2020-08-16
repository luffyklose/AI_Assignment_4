#pragma once
#include "../src/Sprite.h"
#include"UIControl.h"

class HealthBarBorder: public UIControl
{
public:
	HealthBarBorder(Sprite* sp);
	~HealthBarBorder();

	void draw() override;
	void update() override;
	void clean() override;

private:
	Sprite* m_pSp;
};
