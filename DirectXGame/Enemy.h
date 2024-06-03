#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <list>

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

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	void Phase_Approach();
	void Phase_Leave();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

private:
	// メンバ関数ポインタ
	static void (Enemy::*spFuncTable[])();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 敵の移動ベクトル
	Vector3 move_ = {};
};