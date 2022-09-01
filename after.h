#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <list>
#include <memory>
#pragma once

class after {
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialization(Model* model,const Matrix4& position);

	/// <summary>
	/// 更新内の初期化
	/// </summary>
	void State();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	bool Flag() const { return deadFlag; }

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデルデータ
	Model* model_ = nullptr;
	//テクスチャ
	uint32_t textureHandle_ = 0u;
	//入力処理するため
	Input* input_ = nullptr;
	Input* oldinput_ = nullptr;
	//デバックテキスト
	DebugText* debugText_ = nullptr;

	//消滅
	static const int32_t time = 30;
	//デスタイマー
	int32_t deathTimer = time;
	//デスフラグ
	bool deadFlag = false;
};