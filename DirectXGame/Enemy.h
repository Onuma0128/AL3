#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <list>
#include "BaseEnemyState.h"

/// <summary>
/// 敵
/// </summary>

class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);
	Vector3 GetWorldPosition();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	// 状態変更
	void ChangeState(std::unique_ptr<BaseEnemyState> state);

	void EnemyMove(const Vector3& move);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 敵の状態
	std::unique_ptr<BaseEnemyState> state_;

};