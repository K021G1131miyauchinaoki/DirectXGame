#include "Mat.h"
#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	debugText_ = DebugText::GetInstance();
	Fire();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
	worldTransform_.Initialize();
	worldTransform_.translation_ = {4.0f, 3.0f, 20.0f};
	bullet_time = 10;
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
	
	#pragma region 移動処理
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);
	//行列の転送
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

void Enemy::Fire() {
	if (bullet_time-- < 0) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = Vec_rot(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

		//タイマーの数値
		bullet_time = 10;
	}
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Approach_move() {
	//移動
	Vector3 move = {0, 0, -0.2f};
	worldTransform_.translation_ += move;
	//一定の位置に達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::Leave_move() {
	//移動
	Vector3 move = {-0.2f, 0, -0.1f};
	worldTransform_.translation_ += move;
}