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

//カメラクラス
class RailCamera;

///< summary>
///自キャラ
///< summary>
class Player {
  public:
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);

	//更新処理
	void Update();

	//描画処理
	void Draw(ViewProjection& viewProjection);

	//撃つ処理
	void Attack();

	//発射間隔
	static const int kFireInterval = 60;

	//回転処理
	void Rotate();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//弾リストを取得
	std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };

	//半径を取得
	float GetRadius() { return r; }

	//カメラセッター
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//入力処理するため
	Input* input_ = nullptr;
	//デバックテキスト
	DebugText* debugText_ = nullptr;
	//半径
	const float r = 1.0f;
	//発射タイマー
	int32_t bullletTime = 0;

	//カメラクラス
	RailCamera* railCamera_ = nullptr;

};