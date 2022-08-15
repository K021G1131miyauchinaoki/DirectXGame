#include"Skydome.h"
#include"Mat.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	assert(model);

	this->model_ = model;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {100, 100, 100};
}

void Skydome::Update() { 
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection) {
	//3D���f���`��
	model_->Draw(worldTransform_, viewProjection);
}
