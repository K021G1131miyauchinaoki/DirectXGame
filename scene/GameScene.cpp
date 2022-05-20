#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "Vector3.h"
#include <cassert>





GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	worldTransfrom_.Initialize();
	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	#pragma	region	スケーリング
	//xyz方向のスケーリング設定
	worldTransfrom_.scale_ = {1, 1, 1};
	//スケーリング行列を宣言
	Matrix4 matScale;
	matScale.m[0][0] = worldTransfrom_.scale_.x;
	matScale.m[1][1] = worldTransfrom_.scale_.y;
	matScale.m[2][2] = worldTransfrom_.scale_.z;
	matScale.m[3][3] = 1;
	#pragma	endregion

	//単位行列
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	#pragma	region	回転
	//xyz軸周りの回転角を設定
	worldTransfrom_.rotation_ = {1.0f, 1.0f, 1.0f};

	//合成用回転行列を宣言
	Matrix4 matRot=matIdentity;

	Matrix4 matRotX = matIdentity;
	Matrix4 matRotY = matIdentity;
	Matrix4 matRotZ = matIdentity;

	//z軸回転行列を宣言
	matRotZ.m[0][0] = cos(worldTransfrom_.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransfrom_.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransfrom_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransfrom_.rotation_.z);
	
	// x軸回転行列を宣言
	matRotX.m[1][1] = cos(worldTransfrom_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransfrom_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransfrom_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransfrom_.rotation_.x);

	// y軸回転行列を宣言
	matRotY.m[0][0] = cos(worldTransfrom_.rotation_.y);
	matRotY.m[0][2] = -sin( worldTransfrom_.rotation_.y);
	matRotY.m[2][0] = sin(worldTransfrom_.rotation_.y);
	matRotY.m[2][2] = cos(worldTransfrom_.rotation_.y);

	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	
	#pragma	endregion
	
	#pragma region 平行移動
	// x,y,z軸周りの平行移動を設定
	worldTransfrom_.translation_ = {0.0f, 10.0f, 0.0f};
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = worldTransfrom_.translation_.x;
	matTrans.m[3][1] = worldTransfrom_.translation_.y;
	matTrans.m[3][2] = worldTransfrom_.translation_.z;
	matTrans.m[3][3] = 1;

#pragma endregion
	Matrix4 matComb = matScale *= matRot *= matTrans;

	worldTransfrom_.matWorld_ = matIdentity;
	worldTransfrom_.matWorld_ *= matComb;

	//行列の転送
	worldTransfrom_.TransferMatrix();
	
}

void GameScene::Update() { debugCamera_->Update(); }

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
	//描画

	model_->Draw(worldTransfrom_, debugCamera_->GetViewProjection(), textureHandle_);
	// model_->Draw(worldTransfrom_, viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	// x
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {3, 0, 0}, {1, 0, 0, 1});
	// y
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 3, 0}, {0, 1, 0, 1});
	// z
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 0, 3}, {0, 0, 1, 1});
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
