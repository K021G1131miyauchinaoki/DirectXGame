#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "PayerBullet.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <list>
#include <memory>
#pragma once

//�J�����N���X
class RailCamera;

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

	//���ˊԊu
	static const int kFireInterval = 60;

	//��]����
	void Rotate();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//�e���X�g���擾
	std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };

	//���a���擾
	float GetRadius() { return r; }

	//�J�����Z�b�^�[
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

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
	//���a
	const float r = 1.0f;
	//���˃^�C�}�[
	int32_t bullletTime = 0;

	//�J�����N���X
	RailCamera* railCamera_ = nullptr;

};