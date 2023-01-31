#include "PlayerObject.h"
#include "TutorialGame.h"


using namespace NCL;
using namespace CSC8503;

PlayerObject::PlayerObject()
{
	time = 0;
	won = false;


}

void PlayerObject::OnCollisionBegin(GameObject* otherObject)
{
	if (GetCollisionLayer() == Player && otherObject->GetCollisionLayer() == Coin) {
		otherObject->SetBoundingVolume(nullptr);
		otherObject->SetIsActive(false);
		score += 10;
		levelOneItem -= 1;
		levelTwoItem -= 1;
	}
	if (GetCollisionLayer() == Player && otherObject->GetCollisionLayer() == Key) {
		otherObject->SetBoundingVolume(nullptr);
		otherObject->SetIsActive(false);
		key += 1;
	}
	if (key == 1) {
		if (GetCollisionLayer() == Player && otherObject->GetCollisionLayer() == Door) {
			otherObject->SetBoundingVolume(nullptr);
			otherObject->SetIsActive(false);
			key -= 1;
		}
	}
	
}

void PlayerObject::OnCollisionEnd(GameObject* otherObject) {
	if (GetCollisionLayer() == Player && otherObject->GetCollisionLayer() == Enemy) {
		score -= 1;
	}
}

void PlayerObject::Update(float dt)
{
	if (isActive)
	{
		if (!won) {
			time += dt;
		}
	}
}