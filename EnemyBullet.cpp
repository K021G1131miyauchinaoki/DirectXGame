#include"EnemyBullet.h"
#include "Mat.h"
#include <assert.h>
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	velocity_ = velocity;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("red.png");
	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	//�����Ŏ󂯎���ď������W���Z�b�g
	worldTransform_.translation_ = position;
}

void EnemyBullet::Update() {
	worldTransform_.translation_ -= velocity_;
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
	//���Ԍo�߂ŏ���
	if (--deathTimer <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}