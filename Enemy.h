#include "DebugText.h"
#include "Model.h"
#include "EnemyBullet.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <list>
#include <memory>
#include"MathUtility.h"
using namespace MathUtility;


//���@�N���X�̑O���O��
class Player;

//�s���t�F�[�Y
enum class Phase {
	Approach, //�ڋ߂���
	Leave,    //���E����
};
#pragma once
class Enemy {
  public:
	//������
	void Initialize(Model* model, uint32_t textureHandle);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw(ViewProjection& viewProjection);
	//�p�^�[������
	void Approach_move();
	void Leave_move();

	//�e�̔��ˏ���
	void Fire();

	void ApproachInitialize();

	//���ˊԊu
	static const int kFireInterval = 60;

	//�v���C���[�̃Z�b�^�[
	void SetPlayer(Player* player) { player_ = player; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//�f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//���˃^�C�}�[
	int32_t bullletTime=0;
	//���L����
	Player* player_ = nullptr;


};
