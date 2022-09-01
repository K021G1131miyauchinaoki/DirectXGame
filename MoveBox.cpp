#include "Camera.h"
#include "Mat.h"
#include "MoveBox.h"
#include <cassert>
#include <random>

void MoveBox::Initialization(Model* model, uint32_t textureHandle)
{
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();


	worldTransform_.Initialize();

}

void MoveBox::State( const Vector3& scale, const Vector3& trans) {
	worldTransform_.translation_ = trans;
	worldTransform_.scale_ = scale;
	speed = 0.1f;
	fallFlag = false;
}

void MoveBox::Update() {


		//移動
		move = {0, 0, 0};
		

#pragma region 移動処理
		if (worldTransform_.translation_.x>160) {
		    speed = -0.1f;
		} else if (worldTransform_.translation_.x < 140) {
		    speed = 0.1f;
		}
		move = {speed, 0, 0};

		worldTransform_.translation_ += move;

	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
}

void MoveBox::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//ワールド座標を渡す
Vector3 MoveBox::GetWorldPosition() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	// worldTransform_.matWorld_.m;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
