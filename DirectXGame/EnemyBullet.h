#pragma once
#include "Model.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class Player;

class EnemyBullet {
public:
	/// 初期化
	void Initalize(Model* model, const Vector3& position, const Vector3& velocity);
	void SetPlayer(Player* player) { player_ = player; }
	// ワールド座標を取得
	Vector3 GetWorldPosition();

	/// 更新
	void Update();
	// 衝突を検出したら呼び出しされるコールバック関数
	void onCollision();
	bool IsDead() const { return isDead_; }

	/// 描画
	void Draw(const ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデルのポインター
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 弾の速度
	Vector3 velocity_;
	// デスフラグ
	bool isDead_ = false;

	Player* player_ = nullptr;
	float t = 0;
};
