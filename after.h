#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <list>
#include <memory>
#pragma once

class after {
  public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialization(Model* model,const Matrix4& position);

	/// <summary>
	/// �X�V���̏�����
	/// </summary>
	void State();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	bool Flag() const { return deadFlag; }

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f���f�[�^
	Model* model_ = nullptr;
	//�e�N�X�`��
	uint32_t textureHandle_ = 0u;
	//���͏������邽��
	Input* input_ = nullptr;
	Input* oldinput_ = nullptr;
	//�f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;

	//����
	static const int32_t time = 30;
	//�f�X�^�C�}�[
	int32_t deathTimer = time;
	//�f�X�t���O
	bool deadFlag = false;
};