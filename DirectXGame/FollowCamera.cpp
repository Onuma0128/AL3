#include "FollowCamera.h"
#include "imgui.h"

void FollowCamera::Initialize() { viewProjection_.Initialize(); }

void FollowCamera::Reset() {
	if (target_) {
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}

	Vector3 offset = CameraoOffset();
	viewProjection_.translation_ = interTarget_ + offset;

	t_ = 0.0f;
}

Vector3 FollowCamera::CameraoOffset() {
	Vector3 offset = {0.0f, 2.0f, -20.0f};
	Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_.rotation_.y);
	offset = TransformNormal(offset, rotateMatrix);
	return offset;
}

void FollowCamera::Update() {
	XINPUT_STATE joystate;
	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		const float kRotationSpeed = 0.05f;
		destinationTime_ = 0.0f;
		destinationAngleY_ += (float)joystate.Gamepad.sThumbRX / SHRT_MAX * kRotationSpeed;
		if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			destinationAngleY_ = target_->rotation_.y;
			destinationTime_ = 0.0f;
		}
	}
	destinationTime_ += 0.05f;
	viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, destinationTime_);

	if (target_) {
		interTarget_ = Lerp(interTarget_, target_->translation_, t_);
		Vector3 offset = CameraoOffset();
		viewProjection_.translation_ = interTarget_ + offset;
		t_ += 1.0f / 30.0f;
		if (t_ >= 1.0f) {
			t_ = 1.0f;
			// 攻撃処理に入る
			if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				t_ = 0.0f;
			}
			// ダッシュ処理に入る
			if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
				t_ = 0.0f;
			}
		}
	}

	viewProjection_.UpdateMatrix();
	ImGui::Begin("followCamera");
	ImGui::Text("%f", t_);
	ImGui::End();
}
