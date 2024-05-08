#include "Enemy.h"
#include "MT3.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
}

void Enemy::Update() {
	// 敵の移動ベクトル
	Vector3 move = {0, 0, 0};
	// 敵の移動速さ
	const float kEnemySpeed = 0.2f;
	// 移動処理
	worldTransform_.translation_.y = 2.0f;
	move.z -= kEnemySpeed;

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// スケーリング行列の作成
	worldTransform_.matWorld_ = MakeAfineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}