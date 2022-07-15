#include"PayerBullet.h"
#include"Mat.h"
#include<assert.h>
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) { 
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	velocity_ = velocity;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");
	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	//�����Ŏ󂯎���ď������W���Z�b�g
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {
	worldTransform_.translation_ += velocity_;
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
	//���Ԍo�߂ŏ���
	if (--deathTimer <= 0) {
		isDead_ = true;
	}
}

//���[���h���W��n��
Vector3 PlayerBullet::GetBulletPosition() {
	//���W���i�[
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

//�Փ˂�����
void PlayerBullet::OnCollision() { isDead_ = true; }

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}