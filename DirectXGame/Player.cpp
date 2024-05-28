#include "Player.h"
#include "ImGuiManager.h"
#include <cassert>

Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 playerPos) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.translation_ = playerPos;
	worldTransform_.Initialize();
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("2dReticle.png");
	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, Vector2{0.0f, 0.0f}, Vector4{0.0f, 0.0f, 0.0f, 1.0f}, Vector2{0.5f, 0.5f});
}

Vector3 Player::GetWorldPosition() { 
	//ワールド座標を入れる変数
	//ワールド行列の平行成分を取得
	return Transform(Vector3{0, 0, 0}, worldTransform_.matWorld_);
}

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
	worldTransform3DReticle_.parent_ = parent;
}

void Player::Update(const ViewProjection& viewProjection) {
	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクターの旋回処理
	Rotate();
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	// 移動限界座標
	const float kMoveLimiX = 33.0f;
	const float kMoveLimiY = 18.0f;
	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimiX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimiX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimiY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimiY);

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// スケーリング行列の作成
	worldTransform_.matWorld_ = MakeAfineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	inputFloat3[0] = worldTransform_.translation_.x;
	inputFloat3[1] = worldTransform_.translation_.y;
	inputFloat3[2] = worldTransform_.translation_.z;
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, -50.0f, 50.0f);
	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];
	ImGui::End();
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

	// キャラクターの攻撃処理
	Attack();
	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	POINT mousePosition{};
	//マウス座標(スクリーン座標を取得)
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	sprite2DReticle_->SetPosition(Vector2(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0.0f, 1.0f);
	//プロジェクション行列、ビューポート行列を合成して逆行列を計算
	Matrix4x4 matInverseVPV = Multiply(Inverse(matViewport), Inverse(viewProjection.matProjection));
	//スクリーン座標
	Vector3 posNear = Vector3(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 0.0f);
	Vector3 posFar = Vector3(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 1.0f);

	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);
	//マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);
	//カメラから照準オブジェクトの距離
	const float kDistanceTextObject = 100.0f;
	worldTransform3DReticle_.translation_ = Add(posNear, Multiply(kDistanceTextObject, mouseDirection));
	worldTransform3DReticle_.matWorld_ = MakeAfineMatrix(worldTransform3DReticle_.scale_, worldTransform3DReticle_.rotation_, worldTransform3DReticle_.translation_);
	worldTransform3DReticle_.UpdateMatrix();

	/*ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f)", static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle.rotate:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.rotation_.x,
    worldTransform3DReticle_.rotation_.y, worldTransform3DReticle_.rotation_.z);
	ImGui::Text("3DReticle.Pos:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
		worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();*/
}

void Player::onCollision() {}

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	//3Dレティクルを描画
	model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::DrawUI() { 
	sprite2DReticle_->Draw(); 
}

void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
	Vector3 move = {0, 0, 0};

	// 押した方向で移動ベクトルを変換
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = Subtract(worldTransform3DReticle_.translation_, worldTransform_.translation_);
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->SetParent(worldTransform_.parent_);
		newBullet->Initalize(model_, worldTransform_.translation_,velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}