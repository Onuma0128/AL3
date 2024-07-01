#pragma once
#include "Collider.h"
//#include "string.h"
#include "list"

class CollisionManager {
public:
	// コライダーをリストに登録
	void RegisterCollider(Collider* collider);
	// コライダーリストをクリア
	void ClearColliders();
	// 全ての衝突判定を行う
	void CheckAllCollisions();

private:
	// 2つのコライダーの衝突判定を行う
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	// コライダーリスト
	std::list<Collider*> colliders_;


};
