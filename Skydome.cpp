#include "Skydome.h"
#include "Mat.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	assert(model);

	this->model_ = model;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {150, 150, 150};
	worldTransform_.translation_.x = 150;
}

void Skydome::Update() {
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//s—ñ‚Ì“]‘—
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}