#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(const std::vector<Model*>& models) override;
	// 親子付け
	void SetParent(const WorldTransform* parent);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection) override;

private:
	// ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_Gear_;
	WorldTransform worldTransformR_Gear_;
};
