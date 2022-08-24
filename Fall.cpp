#include "Camera.h"
#include "Mat.h"
#include "Fall.h"
#include <cassert>
#include <random>

void Fall::Initialization(Model* model, uint32_t textureHandle, const Vector3& scale, const Vector3& trans) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.scale_ = scale;
	worldTransform_.translation_ = trans;

	worldTransform_.Initialize();
	randNum = 0.2f;
}

void Fall::Update( const Vector3& trans) {
	//移動
	Vector3 move = {0, 0, 0};

	//worldTransform_.translation_ = trans;
	//move.y = minus;
	//minus -= 0.01f;
#pragma region 乱数
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
#pragma endregion

	worldTransform_.translation_.z = value;
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "Floor:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
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