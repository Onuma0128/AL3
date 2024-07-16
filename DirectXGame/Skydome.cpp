#include "Skydome.h"
#include "MT3.h"
#include <cassert>

void Skydome::Initialize(std::unique_ptr<Model>& model) {
	assert(model);
	model_.reset(model.get());
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {10, 10, 10};
}

void Skydome::Update() {
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }