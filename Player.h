#include "DebugText.h"
#include "Input.h"
#include "Audio.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <list>
#include <memory>
#include"after.h"
#pragma once



class Player {
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialization(Model* model, uint32_t textureHandle);

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
	///<summary>
	///ワールド座標を取得
	///< summary>
	Vector3 GetWorldPosition();

	//半径を取得
	float GetRadius() { return r; }

	//ジャンプして衝突したら
	void	UpCollision();

	//側面に当たったら
	void SideCollision();

	void SideCollision2();
	void SideCollision3();
	void SideCollision4();

	void DownCollision();

	//衝突していなかったら
	void OffCollision();


	//衝突したら
	void OnCollision();

	void SwampCollision();


	bool GetFlag() { return flag; };

	std::list<std::unique_ptr<after>> afters_;

	//リストを取得
	std::list<std::unique_ptr<after>>& GetAfters() { return afters_; };

	static const int kFireInterval = 5;

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
	
	//SE
	Audio* audio_ = nullptr;
	uint32_t seJump = 0u;
	uint32_t soundDrain = 0u;

	//半径
	const float r = 1.0f;
	//ジャンプ
	float posY;
	float jumpQuantity;
	float speed;

	//タイム
	int putTime;
	int time;

	int32_t afterTime = 0;

	//フラグ
	bool jumpFlag;
	bool collisionFlag;
	bool flag;
	bool swampFlag;
	int seFlag;

	Vector3 oldTransform;

	//移動
	Vector3 move ;

	float minus ;
};