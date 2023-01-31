#include"GameStatesManager.h"

GameStatesManager::GameStatesManager()
{
	running = true;
	g = new TutorialGame;
	machine = new PushdownMachine(new MenuState(g));
}

GameStatesManager::~GameStatesManager()
{
	delete g;
	delete machine;
}

void GameStatesManager::Update(float dt)
{
	g->UpdateGame(dt);
	if (!machine->Update(dt))
	{
		running = false;
	}
}