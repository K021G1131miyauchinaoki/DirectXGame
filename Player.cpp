#include "Player.h"
#include "Mat.h"
#include <cassert>

void Player::Initialization(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {0, -4, 0};
	worldTransform_.Initialize();
	// worldTransform_.parent_ = &railCamera_->GetWorldMatrix();
	
	jumpQuantity = 2;
	
	time = 5;
	putTime = time;

}

void Player::Update() {

	//移動
	Vector3 move = {0, 0, 0};

	const float speed = 0.2f;
#pragma region 移動処理
	if (input_->PushKey(DIK_UP)) {
		move = {0,  0,speed};
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, 0, -speed};
	} 
	if (input_->PushKey(DIK_LEFT)) {
		move = {-speed, 0, 0};
		if (input_->PushKey(DIK_UP)) {
			move = {-speed, 0, speed};
		} else if (input_->PushKey(DIK_DOWN)) {
			move = {-speed, 0, -speed};
		}
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {speed, 0, 0};
		if (input_->PushKey(DIK_UP)) {
			move = {speed, 0, speed};
		} else if (input_->PushKey(DIK_DOWN)) {
			move = {speed, 0, -speed};
		}
	}
	if (input_->PushKey(DIK_SPACE)&&putTime--==0&&jumpFlag==false) {
		jumpFlag = true;
	}
	if (jumpFlag) {
		posY = jumpQuantity - minus;
		move = {0, posY, 0};
		minus += 0.1f;
		if (worldTransform_.translation_.y<-5) {
			worldTransform_.translation_.y = -5;
			minus = 0;
			jumpFlag = false;
			putTime = time;
		}
	}

	worldTransform_.translation_ += move;
	//上限、下限の設定
	const float kMoveLimitX = 14.0f;
	// const float kMoveLimitY = 8.0f;
	 worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	 worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	// worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	// worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "junmlflag%d", jumpFlag);

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