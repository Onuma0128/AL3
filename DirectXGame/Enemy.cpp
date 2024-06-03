#include "Enemy.h"
#include "MT3.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_.x = 0.0f;
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 10.0f;
	move_ = {0, 0, -0.05f};
}

void (Enemy::*Enemy::spFuncTable[])() = {
		&Enemy::Phase_Approach, //接近
		&Enemy::Phase_Leave     //離脱
};

void Enemy::Update() {

	(this->*spFuncTable[0])();

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
	// スケーリング行列の作成
	worldTransform_.matWorld_ = MakeAfineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Enemy::Phase_Approach() {
	// 移動(ベクトルを加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
	//// スケーリング行列の作成
	worldTransform_.matWorld_ = MakeAfineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		(this->*spFuncTable[1])();
	}
}

void Enemy::Phase_Leave() {
	// 移動(ベクトルを加算)
	move_.x = -0.05f;
	move_.y = 0.05f;
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
