#pragma once
#include "BaseCharacter.h"
#include "Input.h"
#include "MT3.h"
#include "ViewProjection.h"
#include "imgui.h"
#include <optional>

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
	// 通常行動
	void BehaviorRootInitialize();
	// 攻撃行動
	void BehaviorAttackInitialize();
	// ダッシュ
	void BehaviorDashInitialize();
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
	// 通常行動更新
	void BehaviorRootUpdate();
	// 攻撃行動更新
	void BehaviorAttackUpdate();
	// ダッシュ
	void BehaviorDashUpdate();
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
	WorldTransform worldTransformHammer_;
	float destinationAngleY;
	float t_;
	// モデルの配列番号
	uint32_t kModelIndexBody_ = 0;
	uint32_t kModelIndexHead_ = 1;
	uint32_t kModelIndexL_arm_ = 2;
	uint32_t kModelIndexR_arm_ = 3;
	uint32_t kModelIndexHammer_ = 4;
	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// プレイヤーの行動
	enum class Behavior { kRoot, kAttack, kDash };
	// プレイヤーの今の行動
	Behavior behavior_ = Behavior::kRoot;
	// 次の行動リクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	// ダッシュ用ワーク
	struct WorkDash {
		uint32_t dashParameter_ = 0;
	};
	WorkDash workDash_;
};