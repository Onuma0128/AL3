#include "FollowCamera.h"

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	XINPUT_STATE joystate;
	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		const float kRotationSpeed = 0.05f;

		viewProjection_.rotation_.y += (float)joystate.Gamepad.sThumbRX / SHRT_MAX * kRotationSpeed;
	}
	if (target_) {
		Vector3 offset = {0.0f, 2.0f, -20.0f};

		Matrix4x4 makeRotateYMatrix = MakeRotateYMatrix(viewProjection_.rotation_.y);
		offset = TransformNormal(offset, makeRotateYMatrix);

		viewProjection_.translation_ = target_->translation_ + offset;
	}

	viewProjection_.UpdateMatrix();
}
