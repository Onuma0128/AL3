#include "PlayerBullet.h"

void PlayerBullet::Initalize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("sample.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	// 衝突属性を設定
	SetCollisionAttribute(0x00000010);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x11111110);
}

Vector3 PlayerBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行成分を取得
	worldPos = worldTransform_.translation_;

	return worldPos;
}

float PlayerBullet::GetRadius() { return 1.0f; }

void PlayerBullet::Update() {
	worldTransform_.UpdateMatrix();
	worldTransform_.translation_ = Add(velocity_, worldTransform_.translation_);
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::onCollision() { isDead_ = true; }

void PlayerBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }