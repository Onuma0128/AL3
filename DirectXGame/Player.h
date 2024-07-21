#pragma once
#include "Input.h"
#include "MT3.h"
#include "ViewProjection.h"
#include "BaseCharacter.h"
#include "imgui.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(const std::vector<Model*>& models) override;
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
	void Update() override;
	// 浮遊ギミック更新
	void UpdateFloatingGimmick();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection) override;

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
	// モデルの配列番号
	uint32_t kModelIndexBody_ = 0;
	uint32_t kModelIndexHead_ = 1;
	uint32_t kModelIndexL_arm_ = 2;
	uint32_t kModelIndexR_arm_ = 3;
	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
};