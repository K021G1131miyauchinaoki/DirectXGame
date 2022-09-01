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
#pragma region ����
		if (
		  worldTransform_.translation_.z - 3.0f > trans_ &&
		  worldTransform_.translation_.z < trans_ + 5.0f&&!fallFlag) {

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

		//�s��̓]��
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

//�Փ˂�����
void Fall::OnCollision() { flag = false; };