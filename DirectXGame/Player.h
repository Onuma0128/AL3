#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "imgui.h"
#include "MT3.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	Vector3 GetWorldPosition() { return Transform(Vector3{0, 0, 0}, worldTransform_.matWorld_); }
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

private:
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
};