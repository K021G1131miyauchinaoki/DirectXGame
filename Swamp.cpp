#include "Camera.h"
#include "Swamp.h"
#include "Mat.h"
#include <cassert>
#include <random>

void Swamp::Initialization(Model* model, uint32_t textureHandle){
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Swamp::State(const Vector3& scale, const Vector3& trans) {
	worldTransform_.scale_ = scale;
	worldTransform_.translation_ = trans;
	randNum = 0.2f;
	move = 0;
	trans_ = 0;
	speed = 0;
	fallFlag = false;
	flag = true;

}


void Swamp::Update() {
	

	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
	
}

void Swamp::Draw(ViewProjection& viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	
}

//���[���h���W��n��
Vector3 Swamp::GetWorldPosition() {
	//���W���i�[
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	// worldTransform_.matWorld_.m;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
