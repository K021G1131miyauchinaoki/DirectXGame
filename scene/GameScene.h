#pragma once

#include "Audio.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Fall.h"
#include "Floor.h"
#include "Goal.h"
#include "Input.h"
#include "Model.h"
#include "MoveBox.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "Swamp.h"
#include "Skydome.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Player.h>
#include <memory>

//最大数
const int fallNums = 6; /*ステージ1*/
const int boxNums = 4;
const int swampNums = 3;
//ステージ2
const int fallNum2 = 4;
const int boxNum2 = 2;
//ステージ3
const int fallNum3 = 2;  
const int boxNum3 = 2;
const int swampNum3 = 2;



/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision0();
	void CheckAllCollision1();
	void CheckAllCollision2();
	void CheckAllCollision3();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Sprite* sprite_[6];

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	///


	ViewProjection viewProjection_;

	Model* model_ = nullptr;
	//
	uint32_t textureHandle_[6] = {0u, 0u, 0u, 0u, 0u, 0u};
	uint32_t gameHandle[4] = {0u, 0u, 0u,0u};
	uint32_t gameBgm[4] = {0u, 0u, 0u, 0u};
	//bam
	uint32_t soundDrain = 0u;
	//キー
	uint32_t key = 0u;
	//チュートリアルで表示
	uint32_t tutorial = 0u;


	// std::unique_ptr<Player> player;
	//自キャラ
	Player* player_ = nullptr;
	//床
	Floor* floor_ = nullptr;
	//カメラ
	std::unique_ptr<Camera> camera_;
	//半径
	float r;

	int sceneFlag;
	int bgmFlag;
	int stageFalg;

	Vector3 scaleFloor;

	//落下
	
	Fall* fall_[fallNums];

	Vector3 transFall[fallNums] = {
	  {150, 20, -80},
	  {150, 15, -50},
	  {150, 19, -20},
      {150,	13,	 20},
      {150,	22.5,	 21},
      {150,	19,	 40}
	};
	Vector3 transFall2[fallNum2] = {
	  {150, 20,	  -24},
      {150, 20, -18},
      {150, 20,	  -12},
      {150, 20,	   -6 },
    };
	Vector3 transFall3[fallNum3] = {
	  {150, 10.5, -61},
      {150, 21.5, -60}
    };

	//沼
	Swamp* swamp_[swampNums];
	Vector3 transSwamp3[swampNum3] = {
		{150,15,-60},
		{150,15,-15 }
	};
	Vector3 scaleSwamp;

	//箱
	MoveBox* box_[boxNums];
	Vector3 transBox2[boxNum2] = {
	  {130, 15, -30 },
	  {150, 15, 0}
	};
	Vector3 transBox3[boxNum3] = {
	  {130, 15, -25},
      {150, 15, -5  }
    };
	Vector3 scaleBox;

	//ゴール
	Goal* goal_ = nullptr;
	float goalZ;

	//天球
	Skydome* skydome_ = nullptr;
	// 3Dモデル
	Model* modelSkydome_ = nullptr;

};
