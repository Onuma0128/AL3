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
		const float threshold = 0.7f;
		bool isMoving = false;
		// キャラクターの移動ベクトル
		Vector3 move = {
		    .x = (float)joystate.Gamepad.sThumbLX / SHRT_MAX,
		    .y = 0.0f,
		    .z = (float)joystate.Gamepad.sThumbLY / SHRT_MAX,
		};
		if (Length(move) > threshold) {
			isMoving = true;
		}
		move = Normalize(move) * kCharacterSpeed;
		// カメラの動きに合わせて自機を回転
		Matrix4x4 makeCameraRotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		move = Transform(move, makeCameraRotateMatrix);
		if (isMoving) {
			// 移動処理
			worldTransform_.translation_ = worldTransform_.translation_ + move;
			// 自機の動きに合わせて回転
			newRotetionY = std::atan2(move.x, move.z);
			// 補完時間の初期化
			t = 0.3f;
		}
	}
	//最短角度補完
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, newRotetionY, t);

	t += 0.1f;
	if (t > 1.0f) {
		t = 1.0f;
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