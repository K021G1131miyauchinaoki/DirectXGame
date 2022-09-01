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

class MoveBox {
  public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialization(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V���̏�����
	/// </summary>
	void State(const Vector3& scale, const Vector3& trans);

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
	//�ړ�
	Vector3 move;
	float speed;

	int time;
	int timer;


	bool fallFlag;
};