#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include"DebugCamera.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<memory>
#include<Player.h>
#include"Floor.h"
#include"Camera.h"
#include"Fall.h"


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
	void CheckAllCollision();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 
	
	ViewProjection viewProjection_;


	Model* model_ = nullptr;
	uint32_t textureHandle_[2] = {0u,0u};
	//std::unique_ptr<Player> player;
	//自キャラ
	Player* player_ = nullptr;
	//床
	Floor* floor_ = nullptr;
	//カメラ
	std::unique_ptr<Camera> camera_;
	//半径
	float r ;

	Vector3 transFloor;

	//落下
	Fall*fall_=nullptr;


};
