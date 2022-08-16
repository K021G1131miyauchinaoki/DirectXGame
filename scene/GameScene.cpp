#include "GameScene.h"
#include "AxisIndicator.h"
#include "Mat.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "Vector3.h"
#include <cassert>
#include <random>
#define PI (3.14f)

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
	textureHandle_ = TextureManager::Load("sample.png");

	model_ = Model::Create();
	//プレイヤー
	player_ = new Player();
	player_->Initialization(model_, textureHandle_);
	//Player* newPlayer = new Player;
	//player.reset(newPlayer);
	//player->Initialization(model_, textureHandle_);

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Radian_transform(20.0f);
	//アスペクト比
	// viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	// viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
	// viewProjection_.farZ = 53.0f;

	viewProjection_.Initialize();
	// debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する
	// PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() { 
	player_->Update(); 
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
