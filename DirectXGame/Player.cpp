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
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.3f;
	XINPUT_STATE joystate;
	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		// キャラクターの移動ベクトル
		Vector3 move = {
		    .x = (float)joystate.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed,
		    .y = 0.0f,
		    .z = (float)joystate.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed,
		};
		if (move.x != 0.0f || move.z != 0.0f) {
			move = Normalize(move) * kCharacterSpeed;
			// カメラの動きに合わせて自機を回転
			Matrix4x4 makeCameraRotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
			move = Transform(move, makeCameraRotateMatrix);

			// 自機の動きに合わせて回転
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
			// 移動処理
			worldTransform_.translation_ = worldTransform_.translation_ + move;
		}
	}

	worldTransform_.UpdateMatrix();

	ImGui::Begin("player");
	ImGui::Text("translation_.x : %f", worldTransform_.translation_.x);
	ImGui::Text("translation_.z : %f", worldTransform_.translation_.z);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}