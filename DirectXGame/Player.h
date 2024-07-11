#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include "MT3.h"
#include <list>
#include <Sprite.h>
#include <WinApp.h>

class Enemy;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle,Vector3 playerPos);
	// ワールド座標を取得
	Vector3 GetWorldPosition();
	void SetEnemy(std::list<Enemy*> enemy) { enemys_ = enemy; }
	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	void SetParent(const WorldTransform* parent);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const ViewProjection& viewProjection);
	//衝突を検出したら呼び出しされるコールバック関数
	void onCollision();
	//ラープ
	Vector3 Lerp(const Vector3& start, const Vector3& end, float t);
	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullet() const { return bullets_; }

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void DrawUI();

	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// ターゲット攻撃
	/// </summary>
	void TargetAttack();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;
	// ImGuiで値を入力する変数
	float inputFloat3[3];
	//複数弾
	std::list<PlayerBullet*> bullets_;
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	//敵をターゲットしているかどうか
	bool isTargetingEnemy_ = false;
	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	Vector3 old3DReticle_;
	float time_;

	std::list<Enemy*> enemys_;
};