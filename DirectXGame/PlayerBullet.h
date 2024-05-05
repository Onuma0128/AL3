#pragma once
#include "Model.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet {
public:
	/// 初期化
	void Initalize(Model* model, const Vector3& position);

	/// 更新
	void Update();

	/// 描画
	void Draw(const ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデルのポインター
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};