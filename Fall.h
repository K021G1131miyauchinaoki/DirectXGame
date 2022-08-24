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
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialization( Model* model, uint32_t textureHandle, const Vector3& scale, const Vector3& trans);

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
	float posY;
	float minus;

	//乱数用
	float randNum;
};