#pragma once
#include "GameObject.h"
#include "GameWorld.h"
#include "BehaviourAction.h"
#include "BehaviourSequence.h"
#include "BehaviourSelector.h"
#include "BehaviourNodeWithChildren.h"
#include "BehaviourNode.h"
#include "NavigationGrid.h"
#include "NavigationMesh.h"

#include "PlayerObject.h"

//NOT WORKING

using namespace std;


namespace NCL
{
	namespace CSC8503
	{

		class BehaviourTreeEnemy : public PlayerObject
		{
		public:
			BehaviourTreeEnemy(GameWorld* a, const Vector3& initPosition);
			~BehaviourTreeEnemy();

			void Update(float dt) override;

			Vector3 prePos;

			// grid base
			NavigationGrid levelTwoGrid;
			vector<Vector3> levelTwoPathNodes = {};
			float gridBaseL = 0;

			//NavigationMesh* navMesh;
			//// Mesh string path
			//vector<Vector3> meshStringNodes = {};
			//float meshStringL = 0;
			//// Mesh smooth path
			//vector<Vector3> meshSmoothNodes = {};
			//float meshSmoothL = 0;

			vector<Vector3> shortestNodes = {};
			float shortestL = 0;

			int speedFactor = 0;
			int tempIndex = 0;

		protected:

			BehaviourSequence* rootSequence;
			BehaviourSequence* sequence;
			BehaviourSelector* selection;

			GameWorld* worldRef;

			BehaviourState state = Ongoing;

		};
	}
}