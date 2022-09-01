#pragma once

#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <list>
#include <memory>
#pragma once

class Goal {
  public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialization(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V���̏�����
	/// </summary>
	void State( float z);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	///< summary>
	///���[���h���W���擾
	///< summary>
	Vector3 GetWorldPosition();

	float GetFlag() { return flag ; };

	//�Փ˂�����
	void OnCollision();

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f���f�[�^
	Model* model_ = nullptr;
	//�e�N�X�`��
	uint32_t textureHandle_ = 0u;
	//���͏������邽��
	Input* input_ = nullptr;
	//�f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;

	bool flag;
};