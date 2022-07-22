#include"RailCamera.h"
#include"Mat.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	//���[���h�s��
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	//�r���[�v���W�F�N�V����������
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update() { 
	//�ړ��i�x�N�g�������Z�j
	worldTransform_.translation_ += Vector3(0, 0, 0.1f);
	//���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ *= Mat_size(worldTransform_);
	worldTransform_.matWorld_ *= Mat_rot(worldTransform_);
	worldTransform_.matWorld_ *= Mat_move(worldTransform_);
	//���[���h�s��̕��s�ړ��������擾
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	//���[���h�g�����X�t�H�[���̍��W�����Z
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = Vector3Transform(forward, worldTransform_.matWorld_);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target -= viewProjection_.eye += forward; 
	
	//���[���h�g�����X�t�H�[���̊p�x�����Z
	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f
	up = Vector3Transform(up, worldTransform_.matWorld_);
	viewProjection_.up += up;

	//�r���[�v���W�F�N�V�����̍X�V
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(20, 110);
	debugText_->Printf(
	  "RailCamera Pos:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}