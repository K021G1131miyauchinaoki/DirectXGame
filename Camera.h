#include "DebugText.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#pragma once

//自機クラスの前方前言
class Player;

///レールカメラ

class Camera {
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新内の初期化
	/// </summary>
	void State(const Vector3& position,const Vector3& rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	///ビュープロジェクションを取得
	/// </summary>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//ワールド行列を取得
	Matrix4 GetWorldMatrix() const { return worldTransform_.matWorld_; }

	WorldTransform& GetWorldTransform() { return worldTransform_; }

	int GetFlag(int flag) { return flag_ = flag; }

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//デバックテキスト
	DebugText* debugText_ = DebugText::GetInstance();

	Player* player = nullptr;

	int flag_;

	int time;
	int timer;
};

