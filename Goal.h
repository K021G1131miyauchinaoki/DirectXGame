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

class Goal {
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialization(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新内の初期化
	/// </summary>
	void State( float z);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	///< summary>
	///ワールド座標を取得
	///< summary>
	Vector3 GetWorldPosition();

	float GetFlag() { return flag ; };

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

	bool flag;
};