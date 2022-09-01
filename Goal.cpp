#include "Camera.h"
#include "Goal.h"
#include "Mat.h"
#include <cassert>
#include <random>

void Goal::Initialization(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();

}

void Goal::State( float z) {
	worldTransform_.translation_ = {150, 15, z};
	worldTransform_.scale_ = {5.0f, 2.0f, 0.2f};

	flag = false;
}

void Goal::Update() {
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void Goal::Draw(ViewProjection& viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	
}

//���[���h���W��n��
Vector3 Goal::GetWorldPosition() {
	//���W���i�[
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	// worldTransform_.matWorld_.m;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

//�Փ˂�����
void Goal::OnCollision() { flag = true; };