#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"

class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	void SetTarget(const WorldTransform* target) { target_ = target; }
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 追従対象
	const WorldTransform* target_ = nullptr;

};
