#include "Player.h"
#include <cassert>

void Player::Initialize(const std::vector<Model*>& models) {
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	// 親子関係
	SetParent(&worldTransformBase_);

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	worldTransformBase_.translation_ = {0.0f, 0.8f, 0.0f};
	worldTransformBody_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransformHead_.translation_ = {0.0f, 2.3f, 0.0f};
	worldTransformL_arm_.translation_ = {-1.0f, 1.0f, 0.0f};
	worldTransformR_arm_.translation_ = {1.0f, 1.0f, 0.0f};

	InitializeFloatingGimmick();
}

void Player::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f;
}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformBody_.parent_ = parent;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
}

void Player::Update() {
	BaseCharacter::Update();

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
			newRotetionY_ = std::atan2(move.x, move.z);
			// 補完時間の初期化
			t_ = 0.3f;
		}
	}
	// 最短角度補完
	worldTransformBase_.rotation_.y = LerpShortAngle(worldTransformBase_.rotation_.y, newRotetionY_, t_);

	t_ += 0.1f;
	if (t_ > 1.0f) {
		t_ = 1.0f;
	}

	UpdateFloatingGimmick();

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

void Player::UpdateFloatingGimmick() {
	// 浮遊移動のサイクル
	const uint16_t period = 120;
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * pi / period;
	// パラメータ加算
	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * pi);
	// 浮遊の振れ幅
	const float amplitude = 0.2f;
	const float armAmplitude = 0.4f;
	// 浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude;
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * armAmplitude;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * armAmplitude;
}

void Player::Draw(const ViewProjection& viewProjection) {
	// 3Dモデルを描画
	models_[kModelIndexBody_]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead_]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm_]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm_]->Draw(worldTransformR_arm_, viewProjection);
}