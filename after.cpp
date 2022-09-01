#include"after.h"
#include "Mat.h"
#include <assert.h>

void after::Initialization(Model* model, const Matrix4& position) {
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("after.png");
	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	//�����Ŏ󂯎���ď������W���Z�b�g
	worldTransform_.translation_.x = position.m[3][0];
	worldTransform_.translation_.y = 11;
	worldTransform_.translation_.z = position.m[3][2];
	worldTransform_.scale_ = {0.1f, 0.1f, 0.1f};
}
void after::Update() {
	Vector3 size = {0.015,0.02,0.01};
	worldTransform_.scale_ += size;
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
	//���Ԍo�߂ŏ���
	if (--deathTimer <= 0) {
		deadFlag = true;
	}
}

void after::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}