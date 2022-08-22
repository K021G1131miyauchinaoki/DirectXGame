#include "Player.h"
#include"Camera.h"
#include "Mat.h"
#include <cassert>

void Player::Initialization(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {0, -2, 30};
	worldTransform_.Initialize();
	// worldTransform_.parent_ = &railCamera_->GetWorldMatrix();
	
	jumpQuantity = 0.4;
	
	time = 5;
	putTime = time;


	oldTransform.y = worldTransform_.translation_.y + 0.5;

}

void Player::Update() {
	oldTransform = worldTransform_.translation_;


	//移動
	Vector3 move = {0, 0, 0};

	float speed = 0.2f;
	
#pragma region 移動処理
	if (input_->PushKey(DIK_UP)) {
		move = {0,  0,speed};
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, 0, -speed};
	}
	if (input_->PushKey(DIK_SPACE) && putTime-- == 0 && jumpFlag == false && collisionFlag == false){
		jumpFlag = true;
	}
	if (jumpFlag) {
		posY = jumpQuantity - minus;
		move.y = posY;
		minus += 0.01f;
	}



	worldTransform_.translation_ += move;

	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	//行列の転送
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
	debugText_->SetPos(50, 30);
	debugText_->Printf(
	  "minus%f", minus);
	collisionFlag = false;
	debugText_->SetPos(200, 30);
	debugText_->Printf("jumpQuantity%f", jumpQuantity);
	collisionFlag = false;

#pragma endregion
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//ワールド座標を渡す
Vector3 Player::GetWorldPosition() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	// worldTransform_.matWorld_.m;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

//衝突したら
void Player::UpCollision() {
	worldTransform_.translation_.y = oldTransform.y;
	minus = 0;
	jumpFlag = false;
	putTime = time;
	collisionFlag = false;
}


void Player::SideCollision() {
	worldTransform_.translation_.x = oldTransform.x	;
	worldTransform_.translation_.z = oldTransform.z;
	minus -= 0.0001f;
	worldTransform_.translation_.y += minus;
	collisionFlag = false;

}

void Player::DownCollision() {
	
	worldTransform_.translation_ = oldTransform;
	jumpFlag = false;
	collisionFlag = false;
}


void Player::OffCollision() {
	if (!jumpFlag) {
		collisionFlag = true;
		minus -= 0.01f;
		worldTransform_.translation_.y += minus;
		collisionFlag = false;
	}
}