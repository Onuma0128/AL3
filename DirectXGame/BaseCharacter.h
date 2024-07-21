#pragma once
#include <vector>
#include <Model.h>
#include <WorldTransform.h>

class BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデルデータ配列</param>
	virtual void Initialize(const std::vector<Model*>& models);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	virtual void Draw(const ViewProjection& viewProjection);

protected:
	// モデルデータ配列
	std::vector<Model*> models_;
	// ワールド変換データ
	WorldTransform worldTransform_;
};
