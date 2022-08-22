#include "GameScene.h"
#include "AxisIndicator.h"
#include "Mat.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "Vector3.h"
#include <cassert>
#include <random>
#define PI (3.14f)

//矩形(中心基準の場合)
bool CheckHit(
  float x1, float y1, float z1, float w1, float h1, float d1, float x2, float y2, float z2, float w2, float h2, float d2)
{
	float cl1 = x1 - w1;
	float cr1 = cl1 + (w1 * 2);
	float cl2 = x2 - w2;
	float cr2 = cl2 + (w2 * 2);

	if (cr1 <= cl2) {
		return 0;
	}

	if (cr2 <= cl1) {
		return false;
	}

	float cu1 = y1 - h1;
	float cd1 = cu1 + (h1 * 2);
	float cu2 = y2 - h2;
	float cd2 = cu2 + (h2 * 2);

	if (cd1 <= cu2) {
		return false;
	}

	if (cd2 <= cu1) {
		return false;
	}

	float cf1 = z1 - d1;
	float cb1 = cf1 + (d1 * 2);
	float cf2 = z2 - d2;
	float cb2 = cf2 + (d2 * 2);

	if (cb1 <= cf2) {
		return false;
	}

	if (cb2 <= cf1) {
		return false;
	}

	return true;
}

//平面
bool PlaneHit(float a1, float b1, float w1, float h1, float a2, float b2, float w2, float h2,float	c1,float	c2) {
	float cl1 = a1-w1;
	float cr1 = cl1 + (w1 * 2);
	float cl2 = a2 -w2;
	float cr2 = cl2 + (w2 * 2);

	if (cr1 <= cl2) {
		return false;
	}

	if (cr2 <= cl1) {
		return false;
	}

	float cu1 = b1 -h1;
	float cd1 = cu1 + (h1 * 2);
	float cu2 = b2 -h2;
	float cd2 = cu2 + (h2 * 2);

	if (cd1 <= cu2) {
		return false;
	}

	if (cd2 <= cu1) {
		return false;
	}

	float cf1 = c1 - 0.5f;
	float cb1 = c1 + 0.5f;
	float cf2 = c2 - 0.5f;
	float cb2 = c2 + 0.5f;

	if (cb1 <= cf2) {
		return false;
	}
	if (cb2 <= cf1) {
		return false;
	}

	return true;
}

//矩形(6平面)
//int BoxHit( float x1, float y1, float z1, float w1, float h1, float d1, float x2, float y2, float z2,float w2, float h2, float d2,) {
//	PlaneHit(x1 , y1 , w1, h1, x2, y2, w1, h1, z1 + d1, z2 - d2);
//	PlaneHit(x1, y1, w1, h1, x2, y2, w1, h1);
//	PlaneHit(x1, y1, w1, h1, x2, y2, w1, h1);
//	PlaneHit(x1, y1, w1, h1, x2, y2, w1, h1);
//	PlaneHit(x1, y1, w1, h1, x2, y2, w1, h1);
//	PlaneHit(x1, y1, w1, h1, x2, y2, w1, h1);
//}


//ラジアン変換
float Radian_transform(float degree) {
	float radian = degree * (PI / 180.0f);
	return radian;
}
//度数変換
float Degree_transform(float radian) {
	float degree = radian * (180.0f / PI);
	return degree;
}
//
float Clamp(float min, float max, float num) {
	if (num < min) {
		return min;
	} else if (num > max) {
		return max;
	}
	return num;
}

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_[0] = TextureManager::Load("red.png");
	textureHandle_[1] = TextureManager::Load("floor.png");

	model_ = Model::Create();
	//プレイヤー
	player_ = new Player();
	player_->Initialization(model_, textureHandle_[0]);
	
	//床
	vecFloor = {5.0f, 1.0f, 100.0f};
	floor_ = new Floor();
	floor_->Initialization(model_, textureHandle_[1],vecFloor);

	//カメラ
	camera_ = std::make_unique<Camera>();
	camera_->Initialize(Vector3(0, 0, -100), Vector3(0, 0, 0));
	player_->SetParent(&camera_->GetWorldTransform());
	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Radian_transform(20.0f);
	//アスペクト比
	// viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	// viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
	// viewProjection_.farZ = 53.0f;

	viewProjection_.Initialize();

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する
	// PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	
	//半径
	r = 1.0f;
}

void GameScene::Update() { 
	CheckAllCollision();
	camera_->Update();
	viewProjection_.matView = camera_->GetViewProjection().matView;
	viewProjection_.matProjection = camera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();
	player_->Update(); 
	floor_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	floor_->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	#pragma region 自キャラと床の当たり判定
	posA = player_->GetWorldPosition();
	posB = floor_->GetWorldPosition();
	//ジャンプ用
	if (CheckHit(
	      posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, vecFloor.x, vecFloor.y,	vecFloor.z))
	{
		if (PlaneHit(posA.x, posA.y, r, r, posB.x, posB.y, vecFloor.x, vecFloor.y, posB.z + vecFloor.z,posA.z - r)
			||PlaneHit(posA.x, posA.y, r, r, posB.x, posB.y, vecFloor.x, vecFloor.y, posB.z - vecFloor.z,posA.z + r)
			||PlaneHit(posA.z, posA.y, r, r, posB.z, posB.y, vecFloor.z, vecFloor.y, posB.x - vecFloor.x,posA.x + r)
			||PlaneHit(posA.z, posA.y, r, r, posB.z, posB.y, vecFloor.z, vecFloor.y, posB.x + vecFloor.x,posA.x - r)) {
			player_->SideCollision();
			debugText_->SetPos(100, 0);
			debugText_->Printf("1");
		} 
		else if (PlaneHit(posA.x, posA.z, r, r, posB.x, posB.z, vecFloor.x, vecFloor.z, posB.y + vecFloor.y, posA.y - r)) {
			player_->UpCollision();
			debugText_->SetPos(80, 0);
			debugText_->Printf("2");
		} 
		else if (PlaneHit(posA.x, posA.z, r, r, posB.x, posB.z, vecFloor.x, vecFloor.z,posB.y - vecFloor.y, posA.y + r)) {
			player_->DownCollision();
			debugText_->SetPos(100, 0);
			debugText_->Printf("3");
		}
		
		debugText_->SetPos(60, 0);
		debugText_->Printf("5");
		debugText_->SetPos(20, 0);
		debugText_->Printf(
		  "hit");
	}
	//床に当たっていないとき
	else if (!CheckHit(
	      posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, vecFloor.x, vecFloor.y+1.0f,vecFloor.z)) {
		 player_->OffCollision();
	}
	#pragma endregion
}
