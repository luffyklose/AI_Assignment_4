#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"
#include "Renderer.h"
#include "TextureManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
	SDL_RenderPresent(Renderer::Instance()->getRenderer());
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	std::cout << "Clean called on StartScene" << std::endl;
	
	delete m_pStartLabel;
	m_pStartLabel = nullptr;
	
	delete m_pInstructionsLabel;
	m_pInstructionsLabel = nullptr;

	delete m_pStartButton;
	m_pStartButton = nullptr;

	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start()
{
	const SDL_Color color = { 255, 255, 0, 255 };
	/*m_pStartLabel = new Label("START SCENE", "Consolas", 80, blue, glm::vec2(400.0f, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);*/

	//m_pInstructionsLabel = new Label("Press 1 to Play", "Consolas", 40, blue, glm::vec2(400.0f, 120.0f));
	//m_pInstructionsLabel->setParent(this);
	//addChild(m_pInstructionsLabel);

	TextureManager::Instance()->load("../Assets/textures/Logo.png", "Logo");
	m_pLogo = new Image(400, 200);
	addChild(m_pLogo);
	
	m_pNameLabel = new Label("Zihan Xu 101288760 Yiliqi 101289355", "Consolas", 20, color, glm::vec2(400.0f, 550.0f));
	m_pNameLabel->setParent(this);
	addChild(m_pNameLabel);

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(400.0f, 450.0f);

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);
}

