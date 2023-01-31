#include "TutorialGame.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "TextureLoader.h"

#include "PositionConstraint.h"
#include "OrientationConstraint.h"
#include "StateGameObject.h"


using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame()	{
	world		= new GameWorld();
#ifdef USEVULKAN
	renderer	= new GameTechVulkanRenderer(*world);
#else 
	renderer = new GameTechRenderer(*world);
#endif

	physics		= new PhysicsSystem(*world);

	forceMagnitude	= 10.0f;
	useGravity		= true;
	inSelectionMode = false;

	InitialiseAssets();
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void TutorialGame::InitialiseAssets() {
	cubeMesh	= renderer->LoadMesh("cube.msh");
	sphereMesh	= renderer->LoadMesh("sphere.msh");
	charMesh	= renderer->LoadMesh("goat.msh");
	enemyMesh	= renderer->LoadMesh("Keeper.msh");
	bonusMesh	= renderer->LoadMesh("coin.msh");
	capsuleMesh = renderer->LoadMesh("capsule.msh");

	basicTex	= renderer->LoadTexture("checkerboard.png");
	basicShader = renderer->LoadShader("scene.vert", "scene.frag");
	
	InitCamera();
	//InitWorld();
}

TutorialGame::~TutorialGame()	{
	delete cubeMesh;
	delete sphereMesh;
	delete charMesh;
	delete enemyMesh;
	delete bonusMesh;

	delete basicTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}

void TutorialGame::UpdateGame(float dt) {
	if (!menu) {
		if (!inSelectionMode) {
			world->GetMainCamera()->UpdateCamera(dt);
		}

		if (lockedObject != nullptr) {
			Vector3 objPos = lockedObject->GetTransform().GetPosition();
			Vector3 camPos = objPos + lockedOffset;

			Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));

			Matrix4 modelMat = temp.Inverse();

			Quaternion q(modelMat);
			Vector3 angles = q.ToEuler(); //nearly there now!

			world->GetMainCamera()->SetPosition(camPos);
			world->GetMainCamera()->SetPitch(angles.x);
			world->GetMainCamera()->SetYaw(angles.y);
		}

		UpdateKeys();


		//if (useGravity) {
		//	Debug::Print("(G)ravity on", Vector2(5, 95), Debug::RED);
		//}
		//else {
		//	Debug::Print("(G)ravity off", Vector2(5, 95), Debug::RED);
		//}

		RayCollision closestCollision;
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::K) && selectionObject) {
			Vector3 rayPos;
			Vector3 rayDir;

			rayDir = selectionObject->GetTransform().GetOrientation() * Vector3(0, 0, -1);

			rayPos = selectionObject->GetTransform().GetPosition();

			Ray r = Ray(rayPos, rayDir);

			if (world->Raycast(r, closestCollision, true, selectionObject)) {
				if (objClosest) {
					objClosest->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				}
				objClosest = (GameObject*)closestCollision.node;

				objClosest->GetRenderObject()->SetColour(Vector4(1, 0, 1, 1));
			}
		}

		//Debug::DrawLine(Vector3(), Vector3(0, 100, 0), Vector4(1, 0, 0, 1));

		if (levelone) {
			Debug::Print("F1: RESTART", Vector2(75, 10));
			Debug::Print("F3: MAIN MENU", Vector2(75, 20));

			if (GetPlayer()) {
				PlayerObject* player = GetPlayer();
				if (levelOneUseTime) {
					if (!player->lost && !player->won) {
						player->time +=dt;
						player->timeLeft -= dt;
						Debug::Print("LEFT TIME:" + std::to_string((int)floor(player->timeLeft)), Vector2(5, 10));
					}
					if (player->timeLeft <= 0.0) {
						player->lost = true;
						player->won = false;
					}
				}

				Debug::Print("SCORE:" + std::to_string(player->score), Vector2(5, 15));
				Debug::Print("Items Left:" + std::to_string(player->levelOneItem), Vector2(5, 20));
				Debug::Print("Key:" + std::to_string(player->key), Vector2(5, 25));

				if (player->levelOneItem == 0) {
					player->lost = false;
					player->won = true;
				}
				
				
				if (player->won)
				{
					Debug::Print("YOU WON!", Vector2(40, 45));
					string t = "USE " + std::to_string((int)floor(player->time)) + " s";
					Debug::Print(t, Vector2(40, 50));
					world->Clear();
					physics->Clear();
				}
				if (player->lost) {
					Debug::Print("YOU LOSE!", Vector2(40, 50));
					world->Clear();
					physics->Clear();
					//world->GetMainCamera()->SetPosition(Vector3(0, 0, 0));
				}
			}

		}
		if (!levelone) {
			Debug::Print("F1: RESTART", Vector2(75, 10));
			Debug::Print("F3: MAIN MENU", Vector2(75, 20));

			if (GetPlayer()) {
				PlayerObject* player = GetPlayer();
				if (levelOneUseTime) {
					if (!player->lost && !player->won) {
						player->time += dt;
						player->timeLeft -= dt;
						Debug::Print("LEFT TIME:" + std::to_string((int)floor(player->timeLeft)), Vector2(5, 10));
					}
					if (player->timeLeft <= 0.0) {
						player->lost = true;
						player->won = false;
					}
				}

				Debug::Print("SCORE:" + std::to_string(player->score), Vector2(5, 15));
				Debug::Print("Items Left:" + std::to_string(player->levelTwoItem), Vector2(5, 20));
				Debug::Print("Key:" + std::to_string(player->key), Vector2(5, 25));

				if (player->levelTwoItem == 0) {
					player->lost = false;
					player->won = true;
				}

				if (player->won)
				{
					Debug::Print("YOU WON!", Vector2(40, 45));
					string t = "USE " + std::to_string((int)floor(player->time)) + " s";
					Debug::Print(t, Vector2(40, 50));
					world->Clear();
					physics->Clear();
				}
				if (player->lost) {
					Debug::Print("YOU LOSE!", Vector2(40, 50));
					world->Clear();
					physics->Clear();
					//world->GetMainCamera()->SetPosition(Vector3(0, 0, 0));
				}


			}
		}

		SelectObject();
		Debug::Print("And select Sheep and Press L to lock camera", Vector2(5, 90), Debug::RED);
		Debug::Print("Moving:Left,Right,Up,Down  Direction:A,D", Vector2(5, 95), Debug::BLUE);
		//MoveSelectedObject();


		physics->UseGravity(useGravity);
		physics->Update(dt);

		if (testStateObject) {
			testStateObject->Update(dt);

		}

		if (testKey) {
			testKey->Update(dt);

		}

		if (testEnemy) {
			testEnemy->Update(dt);

		}
	}
	else {
		world->GetMainCamera()->SetPosition(Vector3(0, 0, 0));
		Debug::Print("CSC8503 Coursework", Vector2(30, 30), Debug::CYAN);
		Debug::Print(" Level 1 - Press 1", Vector2(30, 40), Debug::GREEN);
		Debug::Print(" Level 2 - Press 2", Vector2(30, 50), Debug::GREEN);
		Debug::Print(" Exit - Press ESC", Vector2(30, 60), Debug::RED);
		physics->Update(dt);
	}

	
	world->UpdateWorld(dt);
	renderer->Update(dt);

	renderer->Render();
	
	Debug::UpdateRenderables(dt);
}

void TutorialGame::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM0)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	//if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
	//	useGravity = !useGravity; //Toggle gravity!
	//	physics->UseGravity(useGravity);
	//}
	

	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}

	if (lockedObject) {
		LockedObjectMovement();
	}
	/*else {
		DebugObjectMovement();
	}*/
}

void TutorialGame::LockedObjectMovement() {
	Matrix4 view		= world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld	= view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();


	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		selectionObject->GetPhysicsObject()->AddForce(fwdAxis * 18);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		selectionObject->GetPhysicsObject()->AddForce(fwdAxis * -18);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		selectionObject->GetPhysicsObject()->AddForce(rightAxis * -18);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		selectionObject->GetPhysicsObject()->AddForce(rightAxis * 18);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NEXT)) {
		selectionObject->GetPhysicsObject()->AddForce(Vector3(0,10,0));
	}
}

void TutorialGame::DebugObjectMovement() {
//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

		/*if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}*/

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM5)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}
}

void TutorialGame::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-100, 40, 100));
	lockedObject = nullptr;
}

void TutorialGame::InitStartMenu() {
	world->ClearAndErase();
	physics->Clear();

	selectionObject = nullptr;

	useGravity = false;
	physics->UseGravity(useGravity);

	menu = true;
	levelone = false;


	lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
	menu = false;
	levelone = true;

	levelOneUseTime = true;

	world->ClearAndErase();
	physics->Clear();

	useGravity = true;
	physics->UseGravity(useGravity);

	
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-30.0f);
	world->GetMainCamera()->SetYaw(180.0f);
	world->GetMainCamera()->SetPosition(Vector3(40, 20, -30));
	lockedObject = nullptr;

	testEnemy = AddEnemy(Vector3(95, -14, 100));
	InitWorldMaze();
	BridgeConstraintTest();
	InitGameExamples();
	InitDefaultFloor();
}

void TutorialGame::InitWorldTwo() {

	menu = false;
	levelone = false;

	levelOneUseTime = true;

	world->ClearAndErase();
	physics->Clear();

	useGravity = true;
	physics->UseGravity(useGravity);


	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-30.0f);
	world->GetMainCamera()->SetYaw(180.0f);
	world->GetMainCamera()->SetPosition(Vector3(40, 20, -30));
	lockedObject = nullptr;


	if (grid == nullptr) {
		grid = new NavigationGrid("TestGrid2.txt");
	}

	int** gridSquare = grid->GetGrid();
	int size = grid->GetSize();
	for (int y = 0; y < grid->GetHeight(); y++) {
		for (int x = 0; x < grid->GetWidth(); x++) {
			if (gridSquare[y][x] == 120) {
				AddCubeToWorld(Vector3(x * size, -15, y * size), Vector3(size / 2, size / 2, size / 2), 0.0f);
			}
		}
	}
	//BridgeConstraintTest();
	AddPlayer(Vector3(45, -12, 25));

	testEnemy = AddEnemy(Vector3(95, -15, 140));

	testStateObject = AddStateObjectToWorld(Vector3(110, -12, 25), 6, 5, 0.01f);

	testKey = AddKeyToWorld();

	InitCubeGridWorld(2, 8, 6.0f, 20.0f);
	InitCubeGridWorld(1, 8, 15.5f, 20.0f);


	AddDoorToWorld(Vector3(95, -12, 145));
	AddFloorToWorld(Vector3(95, -20, 95), Vector3(100, 2, 100));
	

}


/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* TutorialGame::AddFloorToWorld(const Vector3& position, Vector3 floorSize) {
	GameObject* floor = new GameObject();

	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();
	floor->SetCollisionLayer(Floor);

	world->AddGameObject(floor);

	return floor;
}

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple' 
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject();

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetCollisionLayer(Floor);

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddPlayerToWorld(const Vector3& position) {
	float meshSize		= 4.0f;
	float inverseMass	= 0.5f;

	GameObject* character = new GameObject();
	SphereVolume* volume  = new SphereVolume(4.0f);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), charMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();
	character->SetCollisionLayer(Player);
	world->AddGameObject(character);

	return character;
}

PlayerObject* TutorialGame::AddPlayer(const Vector3& position) {
	float meshSize = 4.0f;
	float inverseMass = 0.5f;

	PlayerObject* character = new PlayerObject();
	SphereVolume* volume = new SphereVolume(4.0f);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), charMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();
	character->SetCollisionLayer(Player);
	world->AddGameObject(character);

	levelPlayer = character;
	return character;
}

GameObject* TutorialGame::AddEnemyToWorld(const Vector3& position) {
	float meshSize		= 5.0f;
	float inverseMass	= 0.5f;

	GameObject* character = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();
	character->SetCollisionLayer(Enemy);

	world->AddGameObject(character);
	//enemies.emplace_back(character);

	return character;
}

EnemyObject* TutorialGame::AddEnemy(const Vector3& position) {
	float meshSize = 7.0f;
	float inverseMass = 5.0f;

	EnemyObject* character = new EnemyObject(grid, levelPlayer);

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	character->GetRenderObject()->SetColour(Vector4(0.75, 0, 0, 1));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();
	character->SetCollisionLayer(Enemy);

	world->AddGameObject(character);
	//enemies.emplace_back(character);

	return character;
}

GameObject* TutorialGame::AddBonusToWorld(const Vector3& position) {
	
		GameObject* apple = new GameObject();

		SphereVolume* volume = new SphereVolume(3.0f);
		apple->SetBoundingVolume((CollisionVolume*)volume);
		apple->GetTransform()
			.SetScale(Vector3(0.5, 0.5, 0.5))
			.SetPosition(position);

		apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
		apple->GetRenderObject()->SetColour(Vector4(0.9, 0.9, 0.2, 1));
		apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

		apple->GetPhysicsObject()->SetInverseMass(4.0f);
		apple->GetPhysicsObject()->InitSphereInertia();
		apple->SetCollisionLayer(Coin);
		world->AddGameObject(apple);

		return apple;

}

GameObject* TutorialGame::AddLastBonusToWorld(const Vector3& position) {

	GameObject* apple = new GameObject();

	SphereVolume* volume = new SphereVolume(5.0f);
	apple->SetBoundingVolume((CollisionVolume*)volume);
	apple->GetTransform()
		.SetScale(Vector3(1.0, 1.0, 1.0))
		.SetPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
	apple->GetRenderObject()->SetColour(Vector4(0.9, 0.9, 0.2, 1));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(5.0f);
	apple->GetPhysicsObject()->InitSphereInertia();
	apple->SetCollisionLayer(Coin);
	world->AddGameObject(apple);

	return apple;

}

StateKey* TutorialGame::AddKeyToWorld() {
	Vector3 dimensions = Vector3(2, 2, 2);
	Vector3 position = Vector3(170, -15, 25);
	StateKey* cube = new StateKey();

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(0.8f);
	cube->GetPhysicsObject()->InitCubeInertia();

	cube->GetRenderObject()->SetColour(Vector4(0, 1, 1, 1));
	cube->SetCollisionLayer(Key);

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass) {
	GameObject* capsule = new GameObject();

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform()
		.SetScale(Vector3(radius * 2, halfHeight, radius * 2))
		.SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(capsule);

	return capsule;

}

GameObject* TutorialGame::AddDoorToWorld(const Vector3& position) {
	Vector3 dimensions = Vector3(18, 6, 1);
	GameObject* cube = new GameObject();

	OBBVolume* volume = new OBBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(0);
	cube->GetPhysicsObject()->InitCubeInertia();

	cube->GetRenderObject()->SetColour(Vector4(0, 1, 1, 1));
	cube->SetCollisionLayer(Door);

	world->AddGameObject(cube);

	return cube;
}

StateGameObject* TutorialGame::AddStateObjectToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass) {

	StateGameObject* capsule = new StateGameObject();

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform()
		.SetScale(Vector3(radius * 2, halfHeight, radius * 2))
		.SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->InitCubeInertia();
	capsule->SetCollisionLayer(Floor);

	world->AddGameObject(capsule);

	return capsule;
}

//BehaviourTreeEnemy* TutorialGame::AddBehaviourEnemyToWorld(const Vector3& position) {
//	int radius = 2.0f;
//	float inverseMass = 2.0f;
//	Vector3 sphereSize = Vector3(radius, radius, radius);
//
//	BehaviourTreeEnemy* character = new BehaviourTreeEnemy(world, position);
//
//	SphereVolume* volume = new SphereVolume(radius);
//	character->SetBoundingVolume((CollisionVolume*)volume);
//
//	character->GetTransform()
//		.SetScale(sphereSize)
//		.SetPosition(position);
//
//	character->SetRenderObject(new RenderObject(&character->GetTransform(), sphereMesh, nullptr, basicShader));
//	character->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
//
//	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
//
//	character->GetPhysicsObject()->SetInverseMass(inverseMass);
//	character->GetPhysicsObject()->InitSphereInertia();
//
//	character->SetCollisionLayer(Enemy);
//	world->AddGameObject(character);
//
//	return character;
//}

void TutorialGame::InitWorldMaze() {
	if (grid == nullptr) {
		grid = new NavigationGrid("TestGrid2.txt");
	}

	int** gridSquare = grid->GetGrid();
	int size = grid->GetSize();
	for (int y = 0; y < grid->GetHeight(); y++) {
		for (int x = 0; x < grid->GetWidth(); x++) {
			if (gridSquare[y][x] == 120) {
				AddCubeToWorld(Vector3(x * size, -15, y * size), Vector3(size / 2, size / 2, size / 2), 0.0f);
			}
		}
	}
	
}

void TutorialGame::InitDefaultFloor() {
	AddFloorToWorld(Vector3(95, -20, 70), Vector3(100, 2, 75));
}

void TutorialGame::InitGameExamples() {
	AddPlayer(Vector3(45, -12, 25));
	//AddEnemyToWorld(Vector3(5, 5, 0));
	AddLastBonusToWorld(Vector3(93, -15, 180));
	testStateObject = AddStateObjectToWorld(Vector3(110, -12, 25), 6, 5, 0.01f);
	InitCubeGridWorld(2, 8, 6.0f, 20.0f);
	testKey= AddKeyToWorld();

	AddDoorToWorld(Vector3(95, -12, 145));

}

void TutorialGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(position, radius, 1.0f);
		}
	}
	AddFloorToWorld(Vector3(0, -2, 0), Vector3(100, 2, 75));
}

void TutorialGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);

			if (rand() % 2) {
				AddCubeToWorld(position, cubeDims);
			}
			else {
				AddSphereToWorld(position, sphereRadius);
			}
		}
	}
}

void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	for (int x = 1; x < numCols+1; ++x) {
		for (int z = 1; z < numRows+1; ++z) {
			Vector3 position = Vector3(x * colSpacing, -15.0f, z * rowSpacing * 11);
			AddBonusToWorld(position);
		}
	}
}

//Tutorial 6
void TutorialGame::BridgeConstraintTest() {
	Vector3 cubeSize = Vector3(4, 1, 20);

	float invCubeMass = 5; //how heavy the middle pieces are
	int numLinks = 2;
	float maxDistance = 10; // constraint distance
	float cubeDistance = 7; // distance between links

	Vector3 startPos = Vector3(80, -18, 165);

	GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize, 0);
	

	GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks+2) * cubeDistance, 0, 0), cubeSize, 0);
	

	GameObject* previous = start;

	for (int i = 0; i < numLinks; ++i) {
		GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass);
		
		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;

	}
	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);

}

/*
Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be 
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around. 

*/
bool TutorialGame::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		Debug::Print("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;

				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				return true;
			}
			else {
				return false;
			}
		}
		if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L)) {
			if (selectionObject) {
				if (lockedObject == selectionObject) {
					lockedObject = nullptr;
				}
				else {
					lockedObject = selectionObject;
				}
			}
		}
	}
	else {
		Debug::Print("Press Q to change to select mode!", Vector2(5, 85));
	}
	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/

void TutorialGame::MoveSelectedObject() {
	Debug::Print("Click Force:" + std::to_string(forceMagnitude), Vector2(5, 90));
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject) {
		return;//we haven't selected anything!
	}
	//Push the selected object!
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
			}
		}
	}
}




