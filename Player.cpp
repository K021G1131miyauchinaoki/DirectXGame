#include "Player.h"
#include "Mat.h"
#include <cassert>

void Player::Initialization(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// worldTransform_.translation_ = {0, 0, 24};
	worldTransform_.Initialize();
	// worldTransform_.parent_ = &railCamera_->GetWorldMatrix();
}

void Player::Update() {
	//ˆÚ“®
	Vector3 move = {0, 0, 0};

	const float speed = 0.2f;
#pragma region ˆÚ“®ˆ—
	if (input_->PushKey(DIK_UP)) {
		move = {0, speed, 0};
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, -speed, 0};
	} 
	if (input_->PushKey(DIK_LEFT)) {
		move = {-speed, 0, 0};
		if (input_->PushKey(DIK_UP)) {
			move = {-speed, speed, 0};
		} else if (input_->PushKey(DIK_DOWN)) {
			move = {-speed, -speed, 0};
		}
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {speed, 0, 0};
		if (input_->PushKey(DIK_UP)) {
			move = {speed, speed, 0};
		} else if (input_->PushKey(DIK_DOWN)) {
			move = {speed, -speed, 0};
		}
	}

	worldTransform_.translation_ += move;
	//ãŒÀA‰ºŒÀ‚ÌÝ’è
	const float kMoveLimitX = 14.0f;
	// const float kMoveLimitY = 8.0f;
	 worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	 worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	// worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	// worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//s—ñ‚Ì“]‘—
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

#pragma endregion
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}