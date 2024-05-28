#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	Matrix4x4 GetWorldMatrix() { return worldTransform_.matWorld_; }

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
};
