#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete enemy_;
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvChecker.png");
	// 3Dモデルデータの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	//敵の生成
	enemy_ = new Enemy();
	//敵の初期化
	enemy_->Initialize(model_, textureHandle_);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	//天球の生成
	skydome_ = new Skydome();
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	//天球の初期化
	skydome_->Initialize(modelSkydome_);
}

void GameScene::Update() {
	//天球の更新
	skydome_->Update();
	// 自キャラの更新
	player_->Update();
	// 敵キャラの更新
	enemy_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif
	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
	CheckAllCollisions();
}

void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA, posB;
	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullet();
#pragma region //自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* enemyBullet : enemyBullets) {
		//敵弾の座標
		posB = enemyBullet->GetWorldPosition();
		float posAradius = 1.0f;
		float posBradius = 1.0f;
		if (circleCollision(posA, posB, posAradius, posBradius)) {
			player_->onCollision();
			enemyBullet->onCollision();
		}
	}
#pragma endregion
#pragma region //自弾と敵キャラの当たり判定
	posA = enemy_->GetWorldPosition();
	// 自キャラと敵弾全ての当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		// 敵弾の座標
		posB = playerBullet->GetWorldPosition();
		float posAradius = 1.0f;
		float posBradius = 1.0f;
		if (circleCollision(posA, posB, posAradius, posBradius)) {
			enemy_->onCollision();
			playerBullet->onCollision();
		}
	}
#pragma endregion
#pragma region //自弾と敵弾の当たり判定
	// 自キャラと敵弾全ての当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		// プレイヤーの弾の座標
		posA = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 敵の弾の座標
			posB = enemyBullet->GetWorldPosition();
			float posAradius = 1.0f;
			float posBradius = 1.0f;
			if (circleCollision(posA, posB, posAradius, posBradius)) {
				playerBullet->onCollision();
				enemyBullet->onCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	skydome_->Draw(viewProjection_);
	// 自キャラの描画
	player_->Draw(viewProjection_);
	//敵の描画
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}