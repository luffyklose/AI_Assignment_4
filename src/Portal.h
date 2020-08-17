#pragma once
#include "Sprite.h"

class Portal:public Sprite
{
public:
	Portal();
	~Portal();

	void draw() override;
	void update() override;
	void clean() override;
	int getMaxhealth() override;
private:
	
public:
	
private:
};
