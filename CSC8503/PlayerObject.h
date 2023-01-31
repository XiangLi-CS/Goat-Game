#pragma once
#include "GameObject.h"

namespace NCL
{
	namespace CSC8503
	{
		class PlayerObject : public GameObject
		{
		protected:
			void OnCollisionBegin(GameObject* otherObject) override;
			void OnCollisionEnd(GameObject* otherObject) override;

			virtual void Update(float dt);

		public:
			float time = 0;
			bool won;
			bool lost;
			int score = 0;
			int levelOneItem = 17;
			int levelTwoItem = 24;
			int key = 0;

			

			bool isOpenDoor;

			float timeLeft = 101;
			PlayerObject();



		};

	}
}