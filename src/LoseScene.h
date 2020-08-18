#pragma once

#include "Scene.h"
#include "Label.h"
#include "Button.h"

class LoseScene final : public Scene
{
public:
	LoseScene();
	~LoseScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:
	Label* m_label{};

	Button* m_pRestartButton;
};