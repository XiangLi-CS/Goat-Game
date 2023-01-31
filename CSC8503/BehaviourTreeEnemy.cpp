#include "BehaviourTreeEnemy.h"

//NOT WORKING
BehaviourTreeEnemy::BehaviourTreeEnemy(GameWorld* a, const Vector3& initPosition) {
	worldRef = a;

	prePos = initPosition;

	BehaviourAction* SeekPlayer = new BehaviourAction
	("Seek Player", [&](float dt, BehaviourState state) -> BehaviourState
		{
			PlayerObject* player = worldRef->GetPlayer();

			Vector3 currentPos = GetTransform().GetPosition();
			Vector3 targetPos = player->GetTransform().GetPosition();

			float distToPlayer = (targetPos - currentPos).Length();


			if (state == Initialise)
			{

				return Ongoing;
			}
			else if (state == Ongoing)
			{

				Vector3 startPos = currentPos;

				NavigationGrid levelTwoGrid("TextGrid2.txt");
				NavigationPath gridOutPath;
				bool found = levelTwoGrid.FindPath(startPos, targetPos, gridOutPath);
				levelTwoPathNodes.clear();
				levelTwoPathNodes = gridOutPath.GetWaypoints();
				gridBaseL = 0;
				if (levelTwoPathNodes.size()) {
					for (int i = 0; i < levelTwoPathNodes.size() - 1; i++) {
						gridBaseL += (levelTwoPathNodes[i + 1] - levelTwoPathNodes[i]).Length();
					}
				}

				shortestNodes.clear();
				shortestNodes = levelTwoPathNodes;
				shortestL = gridBaseL;

				tempIndex = shortestNodes.size() - 2;
	
				if (levelTwoPathNodes.size() >= 2) {
					for (int i = 1; i <= levelTwoPathNodes.size() - 1; ++i) {
						Vector3 a = levelTwoPathNodes[i - 1] + Vector3(0, 1, 0);
						Vector3 b = levelTwoPathNodes[i] + Vector3(0, 1, 0);

						Debug::DrawLine(a, b, Vector4(1, 0, 0, 1));
					}
				}
				
				if (shortestNodes.size() <= 0) {
					Vector3 forward = (targetPos - currentPos);
					forward.Normalise();
					prePos = GetTransform().GetPosition();
					GetTransform().SetPosition(GetTransform().GetPosition() + (forward * speedFactor * dt));
					return Ongoing;
				}
				else
				{
					return Failure;
				}
			}
			return state;
		}
	);

	selection = new BehaviourSelector("Loot Selection");
	selection->AddChild(SeekPlayer);

	rootSequence = new BehaviourSequence("Root Sequence");
	rootSequence->AddChild(selection);
}


BehaviourTreeEnemy::~BehaviourTreeEnemy()
{
	delete worldRef;

	delete rootSequence;
	delete sequence;
	delete selection;
}


void BehaviourTreeEnemy::Update(float dt)
{
	if (isActive)
	{
		state = rootSequence->Execute(dt);

		if (state != Ongoing)
		{
			rootSequence->Reset();
		}

		PlayerObject* player = worldRef->GetPlayer();
		if (player->won == true)
		{
			isActive = false;
			won = false;
			lost = true;
		}
		if (player->won == false && player->lost == true) {
			isActive = false;
			lost = false;
			won = true;
		}
	}
}