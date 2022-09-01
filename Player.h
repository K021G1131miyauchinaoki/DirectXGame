#include "DebugText.h"
#include "Input.h"
#include "Audio.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <list>
#include <memory>
#include"after.h"
#pragma once



class Player {
  public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialization(Model* model, uint32_t textureHandle);

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

	void SideCollision2();
	void SideCollision3();
	void SideCollision4();

	void DownCollision();

	//�Փ˂��Ă��Ȃ�������
	void OffCollision();


	//�Փ˂�����
	void OnCollision();

	void SwampCollision();


	bool GetFlag() { return flag; };

	std::list<std::unique_ptr<after>> afters_;

	//���X�g���擾
	std::list<std::unique_ptr<after>>& GetAfters() { return afters_; };

	static const int kFireInterval = 5;

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
	
	//SE
	Audio* audio_ = nullptr;
	uint32_t seJump = 0u;
	uint32_t soundDrain = 0u;

	//���a
	const float r = 1.0f;
	//�W�����v
	float posY;
	float jumpQuantity;
	float speed;

	//�^�C��
	int putTime;
	int time;

	int32_t afterTime = 0;

	//�t���O
	bool jumpFlag;
	bool collisionFlag;
	bool flag;
	bool swampFlag;
	int seFlag;

	Vector3 oldTransform;

	//�ړ�
	Vector3 move ;

	float minus ;
};