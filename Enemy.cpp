#include "Mat.h"
#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	debugText_ = DebugText::GetInstance();
	Fire();

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 3.0f, 10.0f};
}

void Enemy::Update() {
	switch (phase_) {
	case Phase::Approach:
	default:
		Approach_move();
		break;
	case Phase::Leave:
		Leave_move();

		break;
	}
	
	#pragma region �ړ�����
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);
	//�s��̓]��
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

#pragma endregion
	
}

void Enemy::Fire() {
	//�e�̑��x
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = Vec_rot(velocity, worldTransform_.matWorld_);

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Approach_move() {
	//�ړ�
	Vector3 move = {0, 0, -0.2f};
	worldTransform_.translation_ += move;
	//���̈ʒu�ɒB�����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::Leave_move() {
	//�ړ�
	Vector3 move = {-0.2f, 0, -0.2f};
	worldTransform_.translation_ += move;
}