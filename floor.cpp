#include"Floor.h"
#include"Mat.h"

void Floor::Initialization(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	
	worldTransform_.Initialize();
}

void Floor::State(const Vector3& scale) {
	worldTransform_.scale_ = scale;
	worldTransform_.translation_ = {150.0f,10.0f,0.0f};
}

void Floor::Update() {
	//worldTransform_.scale_ = scale;
	//worldTransform_.translation_ = position;
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
	
}

void Floor::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//ワールド座標を渡す
Vector3 Floor::GetWorldPosition() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	// worldTransform_.matWorld_.m;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}