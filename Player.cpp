#include "Player.h"
#include <cassert>
Matrix4 matIdentity() {
	//’PˆÊs—ñ
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	return matIdentity;
}
//•½sˆÚ“®
Matrix4 Mat_move(WorldTransform& w) {
	

	//•½sˆÚ“®s—ñ‚ðéŒ¾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = w.translation_.x;
	matTrans.m[3][1] = w.translation_.y;
	matTrans.m[3][2] = w.translation_.z;
	matTrans.m[3][3] = 1;

	return matTrans;
}
//‰ñ“]
Matrix4 Mat_rot(WorldTransform& w) {
	//’PˆÊs—ñ
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	//‡¬—p‰ñ“]s—ñ‚ðéŒ¾
	Matrix4 matRot = matIdentity;

	Matrix4 matRotX = matIdentity;
	Matrix4 matRotY = matIdentity;
	Matrix4 matRotZ = matIdentity;

	// zŽ²‰ñ“]s—ñ‚ðéŒ¾
	matRotZ.m[0][0] = cos(w.rotation_.z);
	matRotZ.m[0][1] = sin(w.rotation_.z);
	matRotZ.m[1][0] =-sin(w.rotation_.z);
	matRotZ.m[1][1] = cos(w.rotation_.z);

	// xŽ²‰ñ“]s—ñ‚ðéŒ¾
	matRotX.m[1][1] = cos(w.rotation_.x);
	matRotX.m[1][2] = sin(w.rotation_.x);
	matRotX.m[2][1] =-sin(w.rotation_.x);
	matRotX.m[2][2] = cos(w.rotation_.x);

	// yŽ²‰ñ“]s—ñ‚ðéŒ¾
	matRotY.m[0][0] = cos(w.rotation_.y);
	matRotY.m[0][2] =-sin(w.rotation_.y);
	matRotY.m[2][0] = sin(w.rotation_.y);
	matRotY.m[2][2] = cos(w.rotation_.y);

	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return	matRot;
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update() {
	

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
	//ãŒÀA‰ºŒÀ‚ÌÝ’è
	const float kMoveLimitX = 14.0f;
	const float kMoveLimitY = 8.0f;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ *= Rotate() *= Mat_move(worldTransform_);
	//s—ñ‚Ì“]‘—
	worldTransform_.TransferMatrix();
	
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
	
#pragma endregion
	Attack();
	if (bullet_) {
		bullet_->Update();
	}
	
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		bullet_ = newBullet;	
	}
}

Matrix4 Player::Rotate() {

	const float kRot = 0.05f;
	Vector3 Rot = {0, 0, 0};
	if (input_->PushKey(DIK_U)) {
		Rot = {0, kRot, 0};
	} else if (input_->PushKey(DIK_I)) {
		Rot = {0, -kRot, 0};
	}
	worldTransform_.rotation_ += Rot;

	return Mat_rot(worldTransform_);

	//s—ñ‚Ì“]‘—
	//worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}

}