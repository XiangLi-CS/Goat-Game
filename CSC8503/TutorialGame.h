#pragma once
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif
#include "PhysicsSystem.h"

#include "StateGameObject.h"

#include "NavigationGrid.h"

#include"PlayerObject.h"

#include "GameObject.h"

#include"StateKey.h"

//#include"BehaviourTreeEnemy.h"

#include"EnemyObject.h"


namespace NCL {
	namespace CSC8503 {
		class TutorialGame		{
		public:
			TutorialGame();
			~TutorialGame();

			virtual void UpdateGame(float dt);

			void InitStartMenu();
			void InitWorld();
			void InitWorldTwo();

			PlayerObject* AddPlayer(const Vector3& position);

			PlayerObject* GetPlayer() { return levelPlayer; }

			bool levelOneUseTime = false;

			bool levelTwoUseTime = false;



			/*BehaviourTreeEnemy* GetLevelTwoEmeny() { return enemy; }
			BehaviourTreeEnemy* AddBehaviourEnemyToWorld(const Vector3& position);*/

		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			/*
			These are some of the world/object creation functions I created when testing the functionality
			in the module. Feel free to mess around with them to see different objects being created in different
			test scenarios (constraints, collision types, and so on). 
			*/
			void InitGameExamples();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void BridgeConstraintTest();

			void InitWorldMaze();

			void InitDefaultFloor();

			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();
			
			StateGameObject* AddStateObjectToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass);
			StateGameObject * testStateObject;
			

			GameObject* AddFloorToWorld(const Vector3& position, Vector3 floorSize);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);

			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);

			GameObject* AddPlayerToWorld(const Vector3& position);
			GameObject* AddEnemyToWorld(const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position);
			GameObject* AddLastBonusToWorld(const Vector3& position);

			StateKey* AddKeyToWorld(); 
			StateKey* testKey;

			GameObject* AddDoorToWorld(const Vector3& position);	//OBBSphere
			
			EnemyObject* AddEnemy(const Vector3& position);
			EnemyObject* testEnemy;


#ifdef USEVULKAN
			GameTechVulkanRenderer*	renderer;
#else
			GameTechRenderer* renderer;
#endif
			PhysicsSystem*		physics;
			GameWorld*			world;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;
			

			GameObject* selectionObject = nullptr;

			MeshGeometry*	capsuleMesh = nullptr;
			MeshGeometry*	cubeMesh	= nullptr;
			MeshGeometry*	sphereMesh	= nullptr;

			TextureBase*	basicTex	= nullptr;
			ShaderBase*		basicShader = nullptr;

			//Coursework Meshes
			MeshGeometry*	charMesh	= nullptr;
			MeshGeometry*	enemyMesh	= nullptr;
			MeshGeometry*	bonusMesh	= nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset		= Vector3(0, 30, -50);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			bool levelone = false;

			bool menu = false;

			PlayerObject* levelPlayer = nullptr;


			NavigationGrid* grid;

			GameObject* objClosest = nullptr;
		};
	}
}

