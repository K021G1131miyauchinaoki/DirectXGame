#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<assert.h>
#include"PayerBullet.h"
#include<memory>
#include<list>
#pragma once

///< summary>
///���L����
///< summary>
class Player {
  public:
	//������
	void Initialize(Model* model, uint32_t textureHandle);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw(ViewProjection& viewProjection);
	//������
	void Attack();
	//��]����
	void Rotate();
	//���[���h���W���擾
	Vector3 GetWorldPosition();

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���͏������邽��
	Input* input_ = nullptr;
	//�f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;
	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};