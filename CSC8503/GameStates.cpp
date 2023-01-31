#include "GameStates.h"

#include "PushdownMachine.h"
#include "PushdownState.h"

using namespace NCL;
using namespace CSC8503;

PushdownState::PushdownResult MenuState::OnUpdate(float dt, PushdownState** newState) {
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM1))
	{
		*newState = new InitWorldState(g);
		return PushdownResult::Push;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM2))
	{
		*newState = new InitWorldTwoState(g);
		return PushdownResult::Push;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE))
	{
		return PushdownResult::Pop;
	}
	return PushdownResult::NoChange;
}

void MenuState::OnAwake()
{
	g->InitStartMenu();
}

PushdownState::PushdownResult InitWorldState::OnUpdate(float dt, PushdownState** newState)
{

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1))
	{
		g->InitWorld();
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F3))
	{
		return PushdownResult::Pop;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE))
	{
		return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
}

void InitWorldState::OnAwake()
{
	g->InitWorld();
}

PushdownState::PushdownResult InitWorldTwoState::OnUpdate(float dt, PushdownState** newState)
{

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1))
	{
		g->InitWorldTwo();
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F3))
	{
		return PushdownResult::Pop;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE))
	{
		return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
}

void InitWorldTwoState::OnAwake()
{
	g->InitWorldTwo();
}

