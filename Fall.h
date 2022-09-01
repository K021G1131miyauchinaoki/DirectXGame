#pragma once

#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <list>
#include <memory>
#pragma once

class Fall {
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialization( Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新内の初期化
	/// </summary>
	void State( const Vector3& trans);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector3& trans);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	///< summary>
	///ワールド座標を取得
	///< summary>
	Vector3 GetWorldPosition();

	float SetPlayer(const float& trans) { return trans_ = trans; };

	//衝突したら
	void OnCollision();

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデルデータ
	Model* model_ = nullptr;
	//テクスチャ
	uint32_t textureHandle_ = 0u;
	//入力処理するため
	Input* input_ = nullptr;
	//デバックテキスト
	DebugText* debugText_ = nullptr;
	//落下用
	float move;
	float minus;

	//乱数用
	float randNum;

	float trans_;

	bool fallFlag;

	bool flag;
};