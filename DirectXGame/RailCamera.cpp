#include "RailCamera.h"
#include "ImGuiManager.h"
#include "MT3.h"
#include "PrimitiveDrawer.h"

void RailCamera::Initialize() {
	// ワールドトランスフォームの初期化
	worldTransform_.translation_ = {0, 0, -50};
	worldTransform_.rotation_ = {0, 0, 0};
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000;
	viewProjection_.Initialize();
	// 初期時間
	t_ = 0.0f;
}

void RailCamera::Update() {
	// 時間の進行量
	const float dt = 0.001f;
	if (!controlPoints_.empty()) {
		// スプライン上の現在の位置を計算
		Vector3 position = CatmullRomPosition(controlPoints_, t_);
		// 少し先の位置を計算し、注視点を決定
		float nextT = t_ + dt;
		Vector3 lookAtPosition = CatmullRomPosition(controlPoints_, nextT);
		// カメラの位置を更新
		worldTransform_.translation_ = position;
		// カメラのビュー行列を更新
		viewProjection_.matView = LookAt(position, lookAtPosition, {0.0f, 1.0f, 0.0f});
		// 時間を進行
		t_ += dt;
		if (t_ > 1.0f) {
			t_ = 0.0f; // ループさせる場合
		}
		Vector3 velocity = Subtract(lookAtPosition, position);

		worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);
		Matrix4x4 rotateMatrixY = MakeRotateYMatrix(-worldTransform_.rotation_.y);
		Vector3 velocityZ = Transform(velocity, rotateMatrixY);
		worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
	}

	Vector3 move{0.0f, 0.0f, 0.0f};
	Vector3 rad{0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, rad);
	worldTransform_.matWorld_ = MakeAfineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// カメラオブジェクトのワールド行列からビュー行列を計算する
	// viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}

void RailCamera::SetControlPoints(const std::vector<Vector3>& points) { controlPoints_ = points; }