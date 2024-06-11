#include "EnemyBullet.h"
#include "cassert"
#include "Player.h"

void EnemyBullet::Initalize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Block.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};
}

void EnemyBullet::Update() {
	t += 1.0f / 100.0f;
	if (t >= 0.05f) {
		t = 0.0f;
	}
	const float kBulletSpeed = 0.5f;

	Vector3 toPlayer = player_->GetWorldPosition();
	toPlayer = Normalize(Subtract(toPlayer, worldTransform_.translation_));
	velocity_ = Normalize(velocity_);
	velocity_ = Multiply(kBulletSpeed, Slerp(velocity_, toPlayer, t));

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	Matrix4x4 rotateMatrixY = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(velocity_, rotateMatrixY);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	worldTransform_.UpdateMatrix();
	worldTransform_.translation_ = Add(velocity_, worldTransform_.translation_);
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
