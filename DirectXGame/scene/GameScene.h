#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "FollowCamera.h"
#include "Ground.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	// 自キャラ
	std::unique_ptr<Model> modelFighterBody_ = nullptr;
	std::unique_ptr<Model> modelFighterHead_ = nullptr;
	std::unique_ptr<Model> modelFighterL_arm_ = nullptr;
	std::unique_ptr<Model> modelFighterR_arm_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;
	// 敵キャラ
	std::unique_ptr<Model> modelFighterEnemy_ = nullptr;
	std::unique_ptr<Model> modelFighterEnemyGear_ = nullptr;
	std::unique_ptr<Enemy> enemy_ = nullptr;
	// 天球
	std::unique_ptr<Model> skydomeModel_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	// 地面
	std::unique_ptr<Model> groundModel_ = nullptr;
	std::unique_ptr<Ground> ground_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};