#include"Floor.h"
#include"Mat.h"

void Floor::Initialization(Model* model, uint32_t textureHandle, const Vector3&scale) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	
	worldTransform_.scale_ = scale;
	worldTransform_.translation_ = {0.0f,-5.0f,0.0f};

	worldTransform_.Initialize();
	
	
}

void Floor::Update() {
	//worldTransform_.scale_ = scale;
	//worldTransform_.translation_ = position;
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "Floor:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Floor::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//���[���h���W��n��
Vector3 Floor::GetWorldPosition() {
	//���W���i�[
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	// worldTransform_.matWorld_.m;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}