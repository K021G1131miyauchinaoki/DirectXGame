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


//自機クラスの前方前言
class Player;

//行動フェーズ
enum class Phase {
	Approach, //接近する
	Leave,    //離脱する
};
#pragma once
class Enemy {
  public:
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	//更新処理
	void Update();
	//描画処理
	void Draw(ViewProjection& viewProjection);
	//パターン処理
	void Approach_move();
	void Leave_move();

	//弾の発射処理
	void Fire();

	void ApproachInitialize();

	//発射間隔
	static const int kFireInterval = 60;

	//プレイヤーのセッター
	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//デバックテキスト
	DebugText* debugText_ = nullptr;
	//フェーズ
	Phase phase_ = Phase::Approach;
	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//発射タイマー
	int32_t bullletTime=0;
	//自キャラ
	Player* player_ = nullptr;


};
