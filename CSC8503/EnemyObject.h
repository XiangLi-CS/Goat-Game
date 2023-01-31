#pragma once
#include "GameObject.h"
#include"PhysicsSystem.h"
#include"PhysicsObject.h"


namespace NCL {
	namespace CSC8503 {
		class StateMachine;
		class NavigationGrid;
		class EnemyObject : public GameObject {
		public:
			EnemyObject(NavigationGrid* grid, GameObject* target);
			~EnemyObject();

			GameObject* GetTarget() { return target; }
			void SetTarget(GameObject* target) { this->target = target; }
			void MoveToTarget(float dt);
			void CalculatePath();
			std::vector<Vector3> GetPath() { return pathToTarget; }

			virtual void Update(float dt);

		protected:
			GameObject* target;
			NavigationGrid* grid;
			std::vector <Vector3 > pathToTarget;
			float counter;

			StateMachine* stateMachine;
			
		};
	}
}