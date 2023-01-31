#pragma once
#include "GameObject.h"

namespace NCL {
    namespace CSC8503 {
        class StateMachine;
        class StateKey : public GameObject {
        public:
            StateKey();
            ~StateKey();

            virtual void Update(float dt);

        protected:
            void TurnLeft(float dt);
            void TurnRight(float dt);

            StateMachine* stateMachine;
            float counter;
        };
    }
}
