#include"GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "Vector3.h"
#include <cassert>
#include <random>
#define PI (3.14f)

void Mat_move(WorldTransform& w, Vector3 move) {
	//単位行列
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = w.translation_.x;
	matTrans.m[3][1] = w.translation_.y;
	matTrans.m[3][2] = w.translation_.z;
	matTrans.m[3][3] = 1;

	w.matWorld_ = matIdentity;
	w.matWorld_ *= matTrans;

	//行列の転送
	w.TransferMatrix();
}

void Mat(WorldTransform w, Vector3 size, Vector3 rot, Vector3 move) {
#pragma region スケーリング
	// xyz方向のスケーリング設定
	w.scale_ = {size.x, size.y, size.z};
	//スケーリング行列を宣言
	Matrix4 matScale;
	matScale.m[0][0] = w.scale_.x;
	matScale.m[1][1] = w.scale_.y;
	matScale.m[2][2] = w.scale_.z;
	matScale.m[3][3] = 1;
#pragma endregion

	//単位行列
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

#pragma region 回転
	// xyz軸周りの回転角を設定
	w.rotation_ = {rot.x, rot.y, rot.z};

	//合成用回転行列を宣言
	Matrix4 matRot = matIdentity;

	Matrix4 matRotX = matIdentity;
	Matrix4 matRotY = matIdentity;
	Matrix4 matRotZ = matIdentity;

	// z軸回転行列を宣言
	matRotZ.m[0][0] = cos(w.rotation_.z);
	matRotZ.m[0][1] = sin(w.rotation_.z);
	matRotZ.m[1][0] = -sin(w.rotation_.z);
	matRotZ.m[1][1] = cos(w.rotation_.z);

	// x軸回転行列を宣言
	matRotX.m[1][1] = cos(w.rotation_.x);
	matRotX.m[1][2] = sin(w.rotation_.x);
	matRotX.m[2][1] = -sin(w.rotation_.x);
	matRotX.m[2][2] = cos(w.rotation_.x);

	// y軸回転行列を宣言
	matRotY.m[0][0] = cos(w.rotation_.y);
	matRotY.m[0][2] = -sin(w.rotation_.y);
	matRotY.m[2][0] = sin(w.rotation_.y);
	matRotY.m[2][2] = cos(w.rotation_.y);

	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

#pragma endregion

#pragma region 平行移動
	// x,y,z軸周りの平行移動を設定
	w.translation_ = {move.x, move.y, move.z};
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = w.translation_.x;
	matTrans.m[3][1] = w.translation_.y;
	matTrans.m[3][2] = w.translation_.z;
	matTrans.m[3][3] = 1;

#pragma endregion
	Matrix4 matComb = matScale *= matRot *= matTrans;

	w.matWorld_ = matIdentity;
	w.matWorld_ *= matComb;

	//行列の転送
	w.TransferMatrix();
};

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

#pragma region 乱数
	////乱数シード生成器
	// std::random_device seed_gen;
	////メルセンヌ・ツイスターの乱数エンジン
	// std::mt19937_64 engine(seed_gen());
	////乱数範囲の指定
	// std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
	////乱数	（座標）
	// std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
	////乱数　（回転）
	// std::uniform_real_distribution<float> rotDist(-1.9f, 1.9f);
	////乱数エンジンを渡し、指定範囲かっランダムな数値を得る
	// float value = dist(engine);
	// value = rotDist(engine);
	// value = posDist(engine);
#pragma endregion
	//親　0番
	worldTransforms_[0].Initialize();

	//子 1番
	worldTransforms_[1].Initialize();
	worldTransforms_[1].translation_ = {0, 4.5f, 0};
	worldTransforms_[1].parent_ = &worldTransforms_[0];
	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Radian_transform(20.0f);
	//アスペクト比
	// viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	// viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
	// viewProjection_.farZ = 53.0f;

	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	debugCamera_->Update();
#pragma region 視点移動処理
	{
		////視点の移動ベクトル
		// Vector3 move = { 0, 0, 0 };

		////視点の移動の速さ
		// const float kEyeSpeed = 0.2f;

		////押した方向で移動ベクトルを変更
		// if (input_->PushKey(DIK_W)) {
		//	move = { 0, 0, kEyeSpeed };
		// }
		// else if (input_->PushKey(DIK_S)) {
		//	move = { 0, 0, -kEyeSpeed };
		// }
		////視点移動（ベクトルの加算）
		// viewProjection_.eye += move;

		////行列の再計算
		// viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}
#pragma endregion

#pragma region 注視点移動処理
	{
		////視点の移動ベクトル
		// Vector3 move = { 0, 0, 0 };

		////視点の移動の速さ
		// const float kTargetSpeed = 0.2f;

		////押した方向で移動ベクトルを変更
		// if (input_->PushKey(DIK_LEFT)) {
		//	move = { -kTargetSpeed, 0, 0 };
		// }
		// else if (input_->PushKey(DIK_RIGHT)) {
		//	move = { kTargetSpeed, 0, 0 };
		// }
		////視点移動（ベクトルの加算）
		// viewProjection_.target += move;

		////行列の再計算
		// viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
		  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
		  viewProjection_.target.z);
	}
#pragma endregion

#pragma region 上方向回転処理
	//{
	//	//上方向の回転速度[ラジアン/frame]
	//	const float kUpRotSpeed = 0.05f;
	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_SPACE)) {
	//		viewAngle += kUpRotSpeed;
	//		//2πを越えたら0に戻す
	//		viewAngle = fmodf(viewAngle, 3.14f * 2.0f);
	//	}

	//}
	////上方向ベクトルを計算
	// viewProjection_.up = { cosf(viewAngle), sinf(viewAngle), 0.0f };

	////行列の再計算
	// viewProjection_.UpdateMatrix();

	//デバック用表示
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
#pragma endregion

#pragma region Fov変更処理
	{
		//上キーで視野角が広がる
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.fovAngleY += 0.01f;
		}
		//下キーで視野角が狭まる
		if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.fovAngleY -= 0.01f;
		}
		viewProjection_.fovAngleY = Clamp(0.01f, PI, viewProjection_.fovAngleY);
		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf("fovAngleY(Degree):%f", Degree_transform(viewProjection_.fovAngleY));
	}
#pragma endregion

#pragma region クリップ距離変更処理
	{
		//上下キーでニアクリップ距離を増減
		/*if (input_->PushKey(DIK_UP))
		{
		    viewProjection_.nearZ += 0.1f;
		}
		else if(input_->PushKey(DIK_DOWN))
		{
		    viewProjection_.nearZ -= 0.1f;
		}*/
		//行列の再計算
		// viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
	}
#pragma endregion

#pragma region キャラクターの移動処理
	{
		//キャラクターの移動ベクトル
		Vector3 move = {0, 0, 0};
		const float kSpeed = 0.2f;
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kSpeed, 0, 0};
		}

		worldTransforms_[0].translation_ += move;
		Mat_move(worldTransforms_[0], move);

		debugText_->SetPos(50, 150);
		debugText_->Printf(
		  "Trans:(%f,%f,%f)", worldTransforms_[0].translation_.x,
		  worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);
	}

#pragma endregion
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
	//描画

	model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);

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