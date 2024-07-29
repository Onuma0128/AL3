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
	worldTransformHammer_.Initialize();

	worldTransformBase_.translation_ = {0.0f, 0.8f, 0.0f};
	worldTransformBody_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransformHead_.translation_ = {0.0f, 2.3f, 0.0f};
	worldTransformL_arm_.translation_ = {-1.0f, 1.0f, 0.0f};
	worldTransformR_arm_.translation_ = {1.0f, 1.0f, 0.0f};
	worldTransformHammer_.rotation_ = {pi, pi, 0.0f};
	worldTransformHammer_.translation_ = {1.0f, -1.0f, 0.0f};

	InitializeFloatingGimmick();
}

void Player::BehaviorRootInitialize() { InitializeFloatingGimmick(); }

void Player::BehaviorAttackInitialize() { InitializeFloatingGimmick(); }

void Player::BehaviorDashInitialize() {
	workDash_.dashParameter_ = 0;
	worldTransform_.rotation_.y = destinationAngleY;
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformBody_.parent_ = parent;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformHammer_.parent_ = &worldTransformL_arm_;
}

void Player::Update() {
	BaseCharacter::Update();

	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Behavior::kDash:
			BehaviorDashInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Behavior::kDash:
		BehaviorDashUpdate();
		break;
	}

	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();
}

void Player::BehaviorRootUpdate() {
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
			destinationAngleY = std::atan2(move.x, move.z);
			// 補完時間の初期化
			t_ = 0.3f;
		}
	}
	// 最短角度補完
	worldTransformBase_.rotation_.y = LerpShortAngle(worldTransformBase_.rotation_.y, destinationAngleY, t_);

	t_ += 0.1f;
	if (t_ > 1.0f) {
		t_ = 1.0f;
	}

	// 攻撃処理に入る
	if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		behaviorRequest_ = Behavior::kAttack;
	}

	// ダッシュ処理に入る
	if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
		behaviorRequest_ = Behavior::kDash;
	}

	UpdateFloatingGimmick();
}

void Player::BehaviorAttackUpdate() {
	// 攻撃のサイクル
	const uint16_t period = 60;
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * pi / period;
	// パラメータ加算
	floatingParameter_ += step;
	// プレイヤーの動き
	if (floatingParameter_ > step * 30.0f && floatingParameter_ <= step * 40.0f) {
		const float kAttackSpeed = 0.5f;
		Vector3 move = {0.0f, 0.0f, 1.0f};
		move = Normalize(move) * kAttackSpeed;
		// カメラの動きに合わせて自機を回転
		Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransformBase_.rotation_);
		move = Transform(move, rotateMatrix);
		// 移動処理
		worldTransformBase_.translation_ = worldTransformBase_.translation_ + move;
	}
	if (floatingParameter_ < 1.57f * pi) {
		//  腕の振れ幅
		float armAmplitude = pi / 2.0f;
		// 腕の座標に反映
		worldTransformL_arm_.rotation_.x = std::cos(floatingParameter_) * armAmplitude + pi * 1.4f;
		worldTransformR_arm_.rotation_.x = std::cos(floatingParameter_) * armAmplitude + pi * 1.4f;
	}
	if (floatingParameter_ > 2.3f * pi) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorDashUpdate() {
	const float kAttackSpeed = 0.8f;
	Vector3 move = {0.0f, 0.0f, 1.0f};
	move = Normalize(move) * kAttackSpeed;
	// カメラの動きに合わせて自機を回転
	Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransformBase_.rotation_);
	move = Transform(move, rotateMatrix);
	// 移動処理
	worldTransformBase_.translation_ = worldTransformBase_.translation_ + move;
	// ダッシュの時間
	const uint32_t behaviorDashTime = 30;

	worldTransformL_arm_.rotation_.x = 0.5f;
	worldTransformR_arm_.rotation_.x = 0.5f;

	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}
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
	models_[kModelIndexHammer_]->Draw(worldTransformHammer_, viewProjection);
}