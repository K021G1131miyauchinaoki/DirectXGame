#pragma once
#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include"RailCamera.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "Vector3.h"
#include "Vector4.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"Skydome.h"

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

	uint32_t textureHandle_ = 0;
	uint32_t enemyHandle_ = 0;

	Model* model_ = nullptr;

	ViewProjection viewProjection_;

	DebugCamera* debugCamera_ = nullptr;

	Vector3 vec3;
	Vector4 vec4;

	float viewAngle = 0.0f;

	//自キャラ
	Player* player_ = nullptr;

	//敵
	std::unique_ptr<Enemy> enemy_;
	// std::list<std::unique_ptr<Enemy>> enemy_;
	// Enemy* enemy_ = nullptr;

	//天球
	Skydome* skydome_=nullptr;
	//3Dモデル
	Model* modelSkydome_ = nullptr;

	std::unique_ptr<RailCamera> railCamera_;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};