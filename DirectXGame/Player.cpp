#include "Player.h"
#include <cassert>

void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) {
	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;

	// 親子関係
	SetParent(&worldTransformBase_);

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	worldTransformBody_.translation_ = {0.0f, 0.5f, 0.0f};
	worldTransformHead_.translation_ = {0.0f, 2.7f, 0.0f};
	worldTransformL_arm_.translation_ = {-1.0f, 1.2f, 0.0f};
	worldTransformR_arm_.translation_ = {1.0f, 1.2f, 0.0f};

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformBody_.parent_ = parent;
	worldTransformHead_.parent_ = parent;
	worldTransformL_arm_.parent_ = parent;
	worldTransformR_arm_.parent_ = parent;
}

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransformBase_.TransferMatrix();
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
			worldTransformBase_.translation_ = worldTransformBase_.translation_ + move;
			// 自機の動きに合わせて回転
			newRotetionY = std::atan2(move.x, move.z);
			// 補完時間の初期化
			t = 0.3f;
		}
	}
	// 最短角度補完
	worldTransformBase_.rotation_.y = LerpShortAngle(worldTransformBase_.rotation_.y, newRotetionY, t);

	t += 0.1f;
	if (t > 1.0f) {
		t = 1.0f;
	}

	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	ImGui::Begin("player");
	ImGui::Text("translation_.x : %f", worldTransformBase_.translation_.x);
	ImGui::Text("translation_.z : %f", worldTransformBase_.translation_.z);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelHead_->Draw(worldTransformHead_, viewProjection);
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);
}