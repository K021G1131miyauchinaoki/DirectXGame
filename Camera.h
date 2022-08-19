#include "DebugText.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#pragma once

//���@�N���X�̑O���O��
class Player;

///���[���J����

class Camera {
  public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotation);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	///�r���[�v���W�F�N�V�������擾
	/// </summary>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//���[���h�s����擾
	Matrix4 GetWorldMatrix() const { return worldTransform_.matWorld_; }

	WorldTransform& GetWorldTransform() { return worldTransform_; }

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	//�f�o�b�N�e�L�X�g
	DebugText* debugText_ = DebugText::GetInstance();

	Player* player = nullptr;
};