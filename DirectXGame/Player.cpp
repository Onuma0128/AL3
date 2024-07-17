#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.3f;
	XINPUT_STATE joystate;
	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		move.x = (float)joystate.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y = 0.0f;
		move.z = (float)joystate.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
		move = Normalize(move) * kCharacterSpeed;
		Matrix4x4 MakeCameraRotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		move = Transform(move, MakeCameraRotateMatrix);
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

	worldTransform_.translation_ = worldTransform_.translation_ + move;
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}