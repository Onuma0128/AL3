#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <list>
#include "EnemyBullet.h"

//GameSceneの前方宣言
class GameScene;

// 自機クラスの前方宣言
class Player;

/// <summary>
/// 敵
/// </summary>

class Enemy {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);
	void SetPlayer(Player* player) { player_ = player; }
	void SetEnemyPosition(const Vector3& pos);
	// ワールド座標を取得
	Vector3 GetWorldPosition();
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	void Phase_Approach(Vector3& move);
	void Phase_Leave(Vector3& move);
	// 衝突を検出したら呼び出しされるコールバック関数
	void onCollision();
	// 弾リストを取得
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	/// 弾発射
	/// </summary>
	//void Fire();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

public:
	//static const int kFireInterval = 60;
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
	// フェーズ
	Phase phase_ = Phase::Approach;
	//自キャラ
	Player* player_ = nullptr;
	//ゲームシーン
	GameScene* gameScene_ = nullptr;
	// デスフラグ
	bool isDead_ = false;
};