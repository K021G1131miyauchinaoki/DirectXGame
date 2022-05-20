#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "Vector3.h"
#include <cassert>
#include<random>


void mat(WorldTransform	w,float Sx, float Sy, float Sz, float Mx, float My, float Mz, float Tx, float Ty, float Tz ){
	#pragma	region	スケーリング
	// xyz方向のスケーリング設定
	w.scale_ = {Sx,Sy,Sz};
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
	w.rotation_ = {Mx, My, Mz};

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
	w.translation_ = {Tx, Ty, Tz};
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


	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定
	std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
	//乱数	（座標）
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
	//乱数　（回転）
	std::uniform_real_distribution<float> rotDist(-1.9f, 1.9f);
	//乱数エンジンを渡し、指定範囲かっランダムな数値を得る
	float value = dist(engine);
	value = rotDist(engine);
	value = posDist(engine);


	for (WorldTransform&	worldTransform :worldTransforms_) {
		worldTransform.Initialize();
		mat(  worldTransform, 
			/*サイズ*/ 1.0f, 1.0f, 1.0f, 
			/*回転*/ rotDist(engine), rotDist(engine), rotDist(engine),
			/*移動*/ posDist(engine), posDist(engine), posDist(engine));
	}
	//カメラ注視点座標を設定
	viewProjection_.target = {10, 0, 0};
	//カメラ視点座標を設定
	viewProjection_.eye = {0, 0, -50};
	//カメラ上方向ベクトルを設定
	viewProjection_.up = {cosf(3.14f / 4.0f), sinf(3.14f / 4.0f), 0.0f};

	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(1280, 720);

	
	
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	
	
}

void GameScene::Update() { debugCamera_->Update();
	#pragma	region 視点移動処理
	{
		//視点の移動ベクトル
		Vector3 move = {0, 0, 0};

		//視点の移動の速さ
		const float kEyeSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move = {0, 0, kEyeSpeed};
		} else if (input_->PushKey(DIK_S)) {
			move = {0, 0, -kEyeSpeed};
		}
		//視点移動（ベクトルの加算）
		viewProjection_.eye += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}
	#pragma	endregion

	#pragma region 注視点移動処理
	{
		//視点の移動ベクトル
		Vector3 move = {0, 0, 0};

		//視点の移動の速さ
		const float kTargetSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kTargetSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kTargetSpeed, 0, 0};
		}
		//視点移動（ベクトルの加算）
		viewProjection_.target += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
		  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
		  viewProjection_.target.z);
	}
#pragma endregion

	#pragma	region	上方向回転処理
	{
		//上方向の回転速度[ラジアン/frame]
		const float kUpRotSpeed = 0.05f;
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_SPACE)) {
			viewAngle += kUpRotSpeed;
			//2πを越えたら0に戻す
			viewAngle = fmodf(viewAngle, 3.14f * 2.0f);
		}

	}
	//上方向ベクトルを計算
	viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバック用表示
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
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
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}
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
