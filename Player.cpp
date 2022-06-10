#include "Player.h"
#include <cassert>

//平行移動
Matrix4 Mat_move(WorldTransform& w) {
	//単位行列
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

#pragma region 平行移動
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = w.translation_.x;
	matTrans.m[3][1] = w.translation_.y;
	matTrans.m[3][2] = w.translation_.z;
	matTrans.m[3][3] = 1;

#pragma endregion
	w.matWorld_ = matIdentity;
	return w.matWorld_ *= matTrans;
}

Matrix4 Mat_rot(WorldTransform& w) {
	//単位行列
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

#pragma region 回転
	//合成用回転行列を宣言
	Matrix4 matRot = matIdentity;

	Matrix4 matRotX = matIdentity;
	Matrix4 matRotY = matIdentity;
	Matrix4 matRotZ = matIdentity;

	// z軸回転行列を宣言
	matRotZ.m[0][0] = cos(w.rotation_.z);
	matRotZ.m[0][1] = sin(w.rotation_.z);
	matRotZ.m[1][0] =-sin(w.rotation_.z);
	matRotZ.m[1][1] = cos(w.rotation_.z);

	// x軸回転行列を宣言
	matRotX.m[1][1] = cos(w.rotation_.x);
	matRotX.m[1][2] = sin(w.rotation_.x);
	matRotX.m[2][1] =-sin(w.rotation_.x);
	matRotX.m[2][2] = cos(w.rotation_.x);

	// y軸回転行列を宣言
	matRotY.m[0][0] = cos(w.rotation_.y);
	matRotY.m[0][2] =-sin(w.rotation_.y);
	matRotY.m[2][0] = sin(w.rotation_.y);
	matRotY.m[2][2] = cos(w.rotation_.y);

	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

#pragma endregion

	w.matWorld_ = matIdentity;
	return	w.matWorld_ *= matRot;
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
	Rotate();

	Vector3 move = {0, 0, 0};
	
	const float speed = 0.2f;
	#pragma region 移動処理
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
	//上限、下限の設定
	const float kMoveLimitX = 14.0f;
	const float kMoveLimitY = 8.0f;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	
	Mat_move(worldTransform_);
	
	//行列の転送
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

void Player::Rotate() {

	const float kRot = 0.05f;
	Vector3 Rot = {0, 0, 0};
	if (input_->PushKey(DIK_U)) {
		Rot = {0, kRot, 0};
	} else if (input_->PushKey(DIK_I)) {
		Rot = {0, -kRot, 0};
	}
	worldTransform_.rotation_ += Rot;

	Mat_rot(worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}

}