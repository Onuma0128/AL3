#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	void SetControlPoints(const std::vector<Vector3>& points);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	std::vector<Vector3> controlPoints_;
	float t_;
};
