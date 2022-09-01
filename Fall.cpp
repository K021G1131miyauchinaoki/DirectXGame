#include "Camera.h"
#include "Mat.h"
#include "Fall.h"
#include <cassert>
#include <random>

void Fall::Initialization(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Fall::State(const Vector3& trans) {
	worldTransform_.translation_ = trans;

	randNum = 0.2f;
	move = 0;
	minus = 0;
	trans_ = 0;
	fallFlag = false;
	flag = true;
}

void Fall::Update( const Vector3& trans) {
	if (flag) {

		// worldTransform_.translation_ = trans;
#pragma region 乱数
		if (
		  worldTransform_.translation_.z - 3.0f > trans_ &&
		  worldTransform_.translation_.z < trans_ + 5.0f&&!fallFlag) {

			//乱数シード生成器
			std::random_device seed_gen;
			//メルセンヌ・ツイスターの乱数エンジン
			std::mt19937_64 engine(seed_gen());
			//乱数範囲の指定
			std::uniform_real_distribution<float> dist(-randNum, randNum);
			//乱数	（座標）
			std::uniform_real_distribution<float> posDist(trans.z - randNum, trans.z + randNum);
			//乱数　（回転）
			// std::uniform_real_distribution<float> rotDist(-1.9f, 1.9f);
			//乱数エンジンを渡し、指定範囲かっランダムな数値を得る
			float value = dist(engine);
			value = posDist(engine);
			worldTransform_.translation_.z = value;

		} else if (worldTransform_.translation_.z - 0.5f < trans_ + 1.0f) {
			fallFlag = true;
		}
		if (fallFlag) {
			move = minus;
			minus -= 0.01f;
			worldTransform_.translation_.y += move;
		}
		// fallFlag = false;
#pragma endregion

		worldTransform_.matWorld_ = matIdentity();
		worldTransform_.matWorld_ = Mat(worldTransform_);

		//行列の転送
		worldTransform_.TransferMatrix();
	}
	if (worldTransform_.translation_.y<-20) {
		flag = false;
	}
}

void Fall::Draw(ViewProjection& viewProjection) {
	if (flag) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
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

//衝突したら
void Fall::OnCollision() { flag = false; };