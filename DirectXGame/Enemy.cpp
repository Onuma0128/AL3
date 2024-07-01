#include "Enemy.h"
#include "Player.h"
#include "MT3.h"
#include <cassert>

Enemy::~Enemy() { 
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
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

Vector3 Enemy::GetWorldPosition() {
	return Transform(Vector3{0, 0, 0}, worldTransform_.matWorld_); }

float Enemy::GetRadius() { return 1.0f; }

void Enemy::Update() {
	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
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
	for (EnemyBullet* bullet : bullets_){
		bullet->Update();
	}
}

void Enemy::Phase_Approach(Vector3& move) {
	Timer_--;
	if (Timer_ < 0) {
		Fire();
		Timer_ = kFireInterval;
	}
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

void Enemy::onCollision() {}

void Enemy::Fire() {
	// 弾の速度
	const float kBulletSpeed = 0.5f;

	Vector3 worldPlayerPos = player_->GetWorldPosition();
	Vector3 worldEnemyPos = GetWorldPosition();
	Vector3 Vector = Subtract(worldPlayerPos, worldEnemyPos);
	Vector3 normaLizeVector = Normalize(Vector);
	Vector3 velocity = Multiply(kBulletSpeed, normaLizeVector);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initalize(model_,worldTransform_.translation_,velocity);
	newBullet->SetPlayer(player_);

	bullets_.push_back(newBullet);
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (EnemyBullet* bullet : bullets_){
		bullet->Draw(viewProjection);
	}
}
