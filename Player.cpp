#include "Player.h"
#include"Camera.h"
#include "Mat.h"
#include <cassert>


void Player::Initialization(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	
	seJump = audio_->LoadWave("/se/jump.mp3");
	worldTransform_.Initialize();
}

void Player::State() {
	worldTransform_.translation_ = {150, 12, -95};
	jumpQuantity = 0.4f;
	posY = 0;
	time = 5;
	minus = 0;
	putTime = time;
	jumpFlag = false;
	collisionFlag = false;
	flag = true;
	swampFlag = false;
	seFlag = false;
	//移動
	move = {0, 0, 0};

	speed = 0.05f;

	oldTransform.y = worldTransform_.translation_.y;
}

void Player::Update() {
	oldTransform = worldTransform_.translation_;
	afters_.remove_if([](std::unique_ptr<after>& after) { return after->Flag(); });
	if (flag) {

		#pragma region 移動処理
		//移動
		move = {0, 0, 0};
		speed = 0.05f;
		jumpQuantity = 0.4f;
		
		if (swampFlag) {
			speed = 0.025f;
			jumpQuantity = 0.2f;
		}
		if (input_->PushKey(DIK_DOWN) || input_->PushKey(DIK_S)) {
			speed = 0.1f;
			if (swampFlag) {
				speed = 0.05f;
			}
			if (afterTime-- < 0 && jumpFlag == 0) {
				//生成し、初期化
				std::unique_ptr<after> newAfter = std::make_unique<after>();
				newAfter->Initialization(model_, worldTransform_.matWorld_);

				//登録する
				afters_.push_back(std::move(newAfter));
				//タイマーリセット
				afterTime = kFireInterval;
			}
		}

		if (input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_D)) {
			move = {0, 0, speed};

		} else if (input_->PushKey(DIK_LEFT) || input_->PushKey(DIK_A)) {
			move = {0, 0, -speed};
		}
#pragma endregion

#pragma region ジャンプ処理
		//押されたとき
		if (
		  input_->PushKey(DIK_SPACE) && putTime-- == 0 && jumpFlag == false &&
		  collisionFlag == false) {
			jumpFlag = true;
			seFlag = 1;
		}
		if (jumpFlag) {
			posY = jumpQuantity - minus;
			move.y = posY;
			minus += 0.01f;
		}
		#pragma endregion
		worldTransform_.translation_ += move;

		worldTransform_.matWorld_ = matIdentity();
		worldTransform_.matWorld_ = Mat(worldTransform_);
		// worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

		//行列の転送
		worldTransform_.TransferMatrix();

		collisionFlag = false;
		swampFlag = false;
	}
	if (worldTransform_.translation_.y<-20) {
		flag = false;
	}
	//アップデート
	for (std::unique_ptr<after>& after : afters_) {
		after->Update();
	}

	switch (seFlag) {
	case 0:
		audio_->StopWave(soundDrain);
		break;
	case 1:
		soundDrain = audio_->PlayWave(seJump, false);
		seFlag = 3;
		break;
	default:
		break;
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	if (flag) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
		for (std::unique_ptr<after>& after : afters_) {
			after->Draw(viewProjection);
		}
	}
}

//ワールド座標を渡す
Vector3 Player::GetWorldPosition() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	// worldTransform_.matWorld_.m;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

//衝突したら
void Player::OnCollision() { flag = false; }

void Player::UpCollision() {
	worldTransform_.translation_.y = oldTransform.y;
	minus = 0;
	jumpFlag = false;
	seFlag = 0;
	putTime = time;
	collisionFlag = false;
}


void Player::SideCollision() {
	worldTransform_.translation_.x = oldTransform.x	;
	worldTransform_.translation_.z = oldTransform.z;
	speed -= 0.0001f;
	worldTransform_.translation_.y += speed;
	collisionFlag = false;

}

void Player::SideCollision2() {
	worldTransform_.translation_.x = oldTransform.x;
	worldTransform_.translation_.z = oldTransform.z;
}

void Player::SideCollision3() {
	float displace = 0.15f;
	worldTransform_.translation_.x += displace;
	worldTransform_.translation_.z = oldTransform.z;
}

void Player::SideCollision4() {
	float displace = -0.15f;
	worldTransform_.translation_.x += displace;
	worldTransform_.translation_.z = oldTransform.z;
}

void Player::DownCollision() {
	
	worldTransform_.translation_ = oldTransform;
	jumpFlag = false;
	collisionFlag = false;
}

void Player::SwampCollision() {
	swampFlag = true;
}


//衝突しなかったら
void Player::OffCollision() {
	if (!jumpFlag) {
		collisionFlag = true;
		minus -= 0.01f;
		worldTransform_.translation_.y += minus;
		collisionFlag = false;
	}
}