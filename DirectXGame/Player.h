#pragma once
#include "Input.h"
#include "MT3.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "imgui.h"

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
	void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm);
	// 浮遊ギミック初期化
	void InitializeFloatingGimmick();
	// 親子付け
	void SetParent(const WorldTransform* parent);

	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }
	Vector3 GetWorldPosition() { return Transform(Vector3{0, 0, 0}, worldTransformBase_.matWorld_); }
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	// 浮遊ギミック更新
	void UpdateFloatingGimmick();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

private:
	// キーボード入力
	Input* input_ = nullptr;
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	float newRotetionY_;
	float t_;
	// モデル
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
};