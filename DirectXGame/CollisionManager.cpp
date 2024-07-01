#include "CollisionManager.h"

void CollisionManager::RegisterCollider(Collider* collider) { colliders_.push_back(collider); }

void CollisionManager::ClearColliders() { colliders_.clear(); }

void CollisionManager::CheckAllCollisions() {
	for (auto itrA = colliders_.begin(); itrA != colliders_.end(); ++itrA) {
		for (auto itrB = std::next(itrA); itrB != colliders_.end(); ++itrB) {
			CheckCollisionPair(*itrA, *itrB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	if (colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask() ||
		colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) {
		return;
	}
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();
	float radiusA = colliderA->GetRadius();
	float radiusB = colliderB->GetRadius();
	if (circleCollision(posA, posB, radiusA, radiusB)) {
		colliderA->onCollision();
		colliderB->onCollision();
	}
}
