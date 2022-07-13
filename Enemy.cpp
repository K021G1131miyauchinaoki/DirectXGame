#include "Enemy.h"
#include "Mat.h"
#include "Player.h"
#include <cassert>


//������
void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	debugText_ = DebugText::GetInstance();
	ApproachInitialize();
	worldTransform_.Initialize();
	worldTransform_.translation_ = {4.0f, 3.0f, 20.0f};
}

//�X�V
void Enemy::Update() {
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

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
	  "Enemy:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

#pragma endregion
	Fire();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

//�e����
void Enemy::Fire() {
	assert(player_);
	if (bullletTime-- < 0) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity;
		
		Vector3 pPos = player_->GetWorldPosition();
		Vector3 ePos = GetWorldPosition();

		Vector3 len = ePos-pPos;
		velocity = normaleize(len);

		len *= kBulletSpeed;
		//�e�𐶐����A������
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));

		bullletTime = kFireInterval;
	}
}

//�e
void Enemy::ApproachInitialize() { bullletTime = kFireInterval; }

//�s���p�^�[��
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

//���[���h���W��n��
Vector3 Enemy::GetWorldPosition() {
	//���W���i�[
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
//�Փ˂�����
void Enemy::OnCollision() {}

//�`��
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
