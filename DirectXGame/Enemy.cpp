#include "Enemy.h"

void Enemy::Initialize(const std::vector<Model*>& models) {
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	SetParent(&worldTransformBase_);

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_Gear_.Initialize();
	worldTransformR_Gear_.Initialize();

	worldTransformBase_.translation_ = {3.0f, 0.0f, 3.0f};

	worldTransformL_Gear_.rotation_.z = 0.5f * pi;
	worldTransformR_Gear_.rotation_.z = 0.5f * pi;
	worldTransformL_Gear_.translation_ = {2.0f, 1.5f, 0.0f};
	worldTransformR_Gear_.translation_ = {-2.0f, 1.5f, 0.0f};
}

void Enemy::SetParent(const WorldTransform* parent) {
	worldTransformBody_.parent_ = parent;
	worldTransformL_Gear_.parent_ = &worldTransformBody_;
	worldTransformR_Gear_.parent_ = &worldTransformBody_;
}

void Enemy::Update() {
	BaseCharacter::Update();

	worldTransformBase_.rotation_.y += 0.025f;
	worldTransformL_Gear_.rotation_.y += 0.1f;
	worldTransformR_Gear_.rotation_.y += 0.1f;

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.3f;
	Vector3 move = {0.0f, 0.0f, -1.0f};
	move = Normalize(move) * kCharacterSpeed;
	// 向いている方向に合わせて自機を回転
	Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransformBase_.rotation_);
	move = Transform(move, rotateMatrix);
	worldTransformBase_.translation_ = move + worldTransformBase_.translation_;

	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_Gear_.UpdateMatrix();
	worldTransformR_Gear_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformL_Gear_, viewProjection);
	models_[1]->Draw(worldTransformR_Gear_, viewProjection);
}
