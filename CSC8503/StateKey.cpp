#include "StateKey.h"
#include "StateTransition.h"
#include "StateMachine.h"
#include "State.h"
#include "PhysicsObject.h"

using namespace NCL;
using namespace CSC8503;

StateKey::StateKey() {
	counter = 0.0f;
	stateMachine = new StateMachine();

	State* stateA = new State([&](float dt)-> void
		{
			this->TurnLeft(dt);
		}
	);
	State* stateB = new State([&](float dt)-> void
		{
			this->TurnRight(dt);
		}
	);

	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);

	stateMachine->AddTransition(new StateTransition(stateA, stateB, [&]()-> bool
		{
			return this->counter > 3.0f;
		}
	));

	stateMachine->AddTransition(new StateTransition(stateB, stateA, [&]()-> bool
		{
			return this->counter < 0.0f;
		}
	));
}

StateKey::~StateKey() {
	delete stateMachine;
}

void StateKey::Update(float dt) {
	stateMachine->Update(dt);
}

void StateKey::TurnLeft(float dt) {
	GetPhysicsObject()->AddTorque({0, 10, 0});
	counter += dt;
}

void StateKey::TurnRight(float dt) {
	GetPhysicsObject()->AddTorque({0, -10, 0});
	counter -= dt;
}