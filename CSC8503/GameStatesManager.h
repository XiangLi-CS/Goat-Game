#pragma once
#include "PushdownMachine.h"
#include "PushdownState.h"
#include "Window.h"

#include "TutorialGame.h"
#include "GameStates.h"


using namespace NCL;
using namespace CSC8503;

class GameStatesManager
{
public:
	void Update(float dt);
	GameStatesManager();
	~GameStatesManager();

	bool isRunning() { return running; }

protected:
	TutorialGame* g;
	PushdownMachine* machine;
	bool running;
};