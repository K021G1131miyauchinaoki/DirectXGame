#include "DebugText.h"
#include "Input.h"
#include "Model.h"
//#include "PayerBullet.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <list>
#include <memory>
#pragma once


//�J�����N���X
class Camera;


class Player {
  public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialization(Model* model, uint32_t textureHandle);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);
	///<summary>
	///���[���h���W���擾
	///< summary>
	Vector3 GetWorldPosition();

	//���a���擾
	float GetRadius() { return r; }

	//�W�����v���ďՓ˂�����
	void	UpCollision();

	//���ʂɓ���������
	void SideCollision();

	void DownCollision();

	//�Փ˂��Ă��Ȃ�������
	void OffCollision();

	//�J�����Z�b�^�[
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

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
	//���a
	const float r = 1.0f;
	//�W�����v
	float posY;
	float jumpQuantity;
	float minus;

	//�^�C��
	int putTime;
	int time;


	//�t���O
	bool jumpFlag;
	bool collisionFlag;
	

	Vector3 oldTransform;

	//�J�����N���X
	Camera* camera_ = nullptr;

};