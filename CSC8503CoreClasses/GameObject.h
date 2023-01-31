#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

using std::vector;

namespace NCL::CSC8503 {
	enum LAYERS {
		Floor,
		Player,
		Enemy,
		Coin,
		LastCoin,
		Key,
		Door
	};
	class NetworkObject;
	class RenderObject;
	class PhysicsObject;

	class GameObject	{
	public:
		GameObject(std::string name = "");
		~GameObject();

		void SetBoundingVolume(CollisionVolume* vol) {
			boundingVolume = vol;
		}

		const CollisionVolume* GetBoundingVolume() const {
			return boundingVolume;
		}

		bool IsActive() const {
			return isActive;
		}

		void SetIsActive(bool v) {
			isActive = v;
		}

		Transform& GetTransform() {
			return transform;
		}

		RenderObject* GetRenderObject() const {
			return renderObject;
		}

		PhysicsObject* GetPhysicsObject() const {
			return physicsObject;
		}

		NetworkObject* GetNetworkObject() const {
			return networkObject;
		}

		void SetRenderObject(RenderObject* newObject) {
			renderObject = newObject;
		}

		void SetPhysicsObject(PhysicsObject* newObject) {
			physicsObject = newObject;
		}

		const std::string& GetName() const {
			return name;
		}

		void SetName(std::string v) {
			name = v;
		}

		virtual void OnCollisionBegin(GameObject* otherObject) {
			//std::cout << "OnCollisionBegin event occured!\n";
			//if (GetCollisionLayer() == Player && otherObject->GetCollisionLayer() == Coin) {
			//	otherObject->SetBoundingVolume(nullptr);
			//	otherObject->SetIsActive(false);
			//	score += 10;
			//	std::cout << "Score gained: " << score << std::endl;
			//}
			//if (GetCollisionLayer() == Player && otherObject->GetCollisionLayer() == LastCoin) {
			//	otherObject->SetBoundingVolume(nullptr);
			//	otherObject->SetIsActive(false);
			//	score += 10;
			//	std::cout << "Score gained: " << score << std::endl;
			//	std::cout << "You win" << std::endl;
			//}
			//if (GetCollisionLayer() == Player && otherObject->GetCollisionLayer() == Enemy) {
			//	otherObject->SetBoundingVolume(nullptr);
			//	otherObject->SetIsActive(false);
			//}
			//if (GetCollisionLayer() == Enemy && otherObject->GetCollisionLayer() == Coin) {
			//	otherObject->SetBoundingVolume(nullptr);
			//	otherObject->SetIsActive(false);
			//}
			

		}

		virtual void OnCollisionEnd(GameObject* otherObject) {
			//std::cout << "OnCollisionEnd event occured!\n";
		}

		bool GetBroadphaseAABB(Vector3&outsize) const;

		void UpdateBroadphaseAABB();

		void SetWorldID(int newID) {
			worldID = newID;
		}

		int		GetWorldID() const {
			return worldID;
		}

		void	SetCollisionLayer(int newLayer) {
			layer = newLayer;
		}

		int		GetCollisionLayer() const {
			return layer;
		}

		int score = 0;

	protected:
		Transform			transform;		//gives us the world position and size of a GameObject, and can give us both the local and world matrices we looked at in the previous module with scene graphs.

		CollisionVolume*	boundingVolume;		//allows us to define a simple shape for collision detection and visibility purposes
		PhysicsObject*		physicsObject;
		RenderObject*		renderObject;
		NetworkObject*		networkObject;

		bool		isActive;
		int			worldID;
		int			layer;
		bool		isOpenDoor;
		
		std::string	name;

		Vector3 broadphaseAABB;
	};
}

