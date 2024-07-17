#include "MT3.h"
#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, -1.0f, 0.0f};
	worldTransform_.scale_ = {100, 100, 100};
}

void Ground::Update() {
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Ground::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }