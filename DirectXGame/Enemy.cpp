#include "Enemy.h"
#include "MT3.h"
#include <cassert>
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_.x = 0.0f;
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 10.0f;
	ChangeState(std::make_unique<EnemyStateApproach>(this));
}

Vector3 Enemy::GetWorldPosition() { 
	return Transform(Vector3{0, 0, 0}, worldTransform_.matWorld_); 
}

void Enemy::Update() {
	if (worldTransform_.translation_.z <= 0) {
		ChangeState(std::make_unique<EnemyStateLeave>(this));
	}

	state_->Update();

	// スケーリング行列の作成
	worldTransform_.matWorld_ = MakeAfineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> state) { state_ = std::move(state); }

void Enemy::EnemyMove(const Vector3& move) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
