#pragma once
#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	void SetTarget(const WorldTransform* target) {
		target_ = target;
		Reset();
	}
	Vector3 GetTarget() { return target_->translation_; }
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	void Reset();
	Vector3 CameraoOffset();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 追従対象
	const WorldTransform* target_ = nullptr;
	// 追従対象の残像
	Vector3 interTarget_ = {};
	float t_;
	// 目標角度
	float destinationAngleY_ = 0.0f;
	float destinationTime_ = 0.0f;
};
