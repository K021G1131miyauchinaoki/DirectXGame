#include "Player.h"
#include"Mat.h"
#include <cassert>



void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update() {
	//‰ñ“]
	Rotate();
	//ˆÚ“®
	Vector3 move = {0, 0, 0};
	
	const float speed = 0.2f;
	#pragma region ˆÚ“®ˆ—
	if (input_->PushKey(DIK_UP)) {
		move = {0, speed, 0};
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, -speed, 0};
	} else if (input_->PushKey(DIK_LEFT)) {
		move = {-speed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {speed, 0, 0};
	}
	
	worldTransform_.translation_ += move;
	//ãŒÀA‰ºŒÀ‚Ìİ’è
	const float kMoveLimitX = 14.0f;
	const float kMoveLimitY = 8.0f;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);
	//s—ñ‚Ì“]‘—
	worldTransform_.TransferMatrix();
	
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
	
#pragma endregion
	Attack();
	for (std::unique_ptr<PlayerBullet> & bullet : bullets_)
	{
		bullet->Update();
	}
	
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
		//’e‚Ì‘¬“x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//‘¬“xƒxƒNƒgƒ‹‚ğ©‹@‚ÌŒü‚«‚É‡‚í‚¹‚Ä‰ñ“]‚³‚¹‚é
		velocity = Vec_rot(velocity, worldTransform_.matWorld_);

		//’e‚ğ¶¬‚µA‰Šú‰»
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//’e‚ğ“o˜^‚·‚é
		bullets_.push_back(std::move(newBullet));	
	}
}

void Player::Rotate() {

	const float kRot = 0.05f;
	Vector3 Rot = {0, 0, 0};
	if (input_->PushKey(DIK_U)) {
		Rot = {0, kRot, 0};
	} else if (input_->PushKey(DIK_I)) {
		Rot = {0, -kRot, 0};
	}
	worldTransform_.rotation_ += Rot;
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (std::unique_ptr<PlayerBullet>&bullet:bullets_) {
		bullet->Draw(viewProjection);
	}

}