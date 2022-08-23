#include "Camera.h"
#include "Mat.h"
#include "Fall.h"
#include <cassert>

void Fall::Initialization(Model* model, uint32_t textureHandle, const Vector3& scale) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.scale_ = scale;
	worldTransform_.translation_ = {150.0f, 10.0f, 0.0f};

	worldTransform_.Initialize();
}

void Fall::Update() {
	// worldTransform_.scale_ = scale;
	// worldTransform_.translation_ = position;
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
	// debugText_->SetPos(50, 90);
	// debugText_->Printf(
	//   "Floor:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	//   worldTransform_.translation_.z);
}

void Fall::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//ワールド座標を渡す
Vector3 Fall::GetWorldPosition() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	// worldTransform_.matWorld_.m;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}