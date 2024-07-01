#pragma once
#include "Model.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include "Collider.h"

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet : public Collider{
public:
	/// 初期化
	void Initalize(Model* model, const Vector3& position, const Vector3& velocity);
	// ワールド座標を取得
	Vector3 GetWorldPosition() override;
	float GetRadius() override;

	/// 更新
	void Update();
	// 衝突を検出したら呼び出しされるコールバック関数
	void onCollision() override;
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
	// 弾の寿命
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};