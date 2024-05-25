#include "Enemy.h"
#include "Player.h"
#include "MT3.h"
#include <cassert>

Enemy::~Enemy() { 

}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_.x = 10.0f;
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 50.0f;
}

void Enemy::SetEnemyPosition(const Vector3& pos) { worldTransform_.translation_ = pos; }

Vector3 Enemy::GetWorldPosition() { 
	return Transform(Vector3{0, 0, 0}, worldTransform_.matWorld_); 
}

void Enemy::Update() {
	// 敵の移動ベクトル
	Vector3 move_ = {0, 0, -0.01f};
	//移動処理
	switch (phase_) { 
	case Phase::Approach:
	default:
		Phase_Approach(move_);
		break;
    case Phase::Leave:
		Phase_Leave(move_);
		break;
	}
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Enemy::Phase_Approach(Vector3& move) {
	// 移動(ベクトルを加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	//// スケーリング行列の作成
	worldTransform_.matWorld_ = MakeAfineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Phase_Leave(Vector3& move) {
	// 移動(ベクトルを加算)
	move.x = -0.1f;
	move.y = 0.1f;
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Enemy::onCollision() { isDead_ = true; }

void Enemy::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
