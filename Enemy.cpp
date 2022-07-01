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
	
	#pragma region ˆÚ“®ˆ—
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);
	//s—ñ‚Ì“]‘—
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

#pragma endregion
	
}

void Enemy::Fire() {
	//’e‚Ì‘¬“x
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//‘¬“xƒxƒNƒgƒ‹‚ğ©‹@‚ÌŒü‚«‚É‡‚í‚¹‚Ä‰ñ“]‚³‚¹‚é
	velocity = Vec_rot(velocity, worldTransform_.matWorld_);

	//’e‚ğ¶¬‚µA‰Šú‰»
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//’e‚ğ“o˜^‚·‚é
	bullets_.push_back(std::move(newBullet));
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Approach_move() {
	//ˆÚ“®
	Vector3 move = {0, 0, -0.2f};
	worldTransform_.translation_ += move;
	//ˆê’è‚ÌˆÊ’u‚É’B‚µ‚½‚ç—£’E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::Leave_move() {
	//ˆÚ“®
	Vector3 move = {-0.2f, 0, -0.2f};
	worldTransform_.translation_ += move;
}