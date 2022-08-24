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
	//�ړ�
	Vector3 move = {0, 0, 0};

	//worldTransform_.translation_ = trans;
	//move.y = minus;
	//minus -= 0.01f;
#pragma region ����
	//�����V�[�h������
	 std::random_device seed_gen;
	//�����Z���k�E�c�C�X�^�[�̗����G���W��
	 std::mt19937_64 engine(seed_gen());
	//�����͈͂̎w��
	 std::uniform_real_distribution<float> dist(-randNum, randNum);
	//����	�i���W�j
	 std::uniform_real_distribution<float> posDist(trans.z - randNum, trans.z + randNum);
	//�����@�i��]�j
	// std::uniform_real_distribution<float> rotDist(-1.9f, 1.9f);
	//�����G���W����n���A�w��͈͂��������_���Ȑ��l�𓾂�
	 float value = dist(engine);
	 value = posDist(engine);
#pragma endregion

	worldTransform_.translation_.z = value;
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "Floor:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Fall::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//���[���h���W��n��
Vector3 Fall::GetWorldPosition() {
	//���W���i�[
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	// worldTransform_.matWorld_.m;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}