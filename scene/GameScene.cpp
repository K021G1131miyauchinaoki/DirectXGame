#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "Vector3.h"
#include <cassert>
#include <random>
#include"Mat.h"
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

GameScene::~GameScene() {
	delete model_;
	//
	 //delete debugCamera_;
	//
	delete player_;

	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyHandle_ = TextureManager::Load("enemy.png");

	model_ = Model::Create();
	//プレイヤー
	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	//敵
	Enemy* newEnemy = new Enemy;
	newEnemy->Initialize(model_, enemyHandle_);
	//敵の登録
	enemy_.reset(newEnemy);
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//天球
	//3Dモデル生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);
	railCamera_ = std::make_unique<RailCamera>();

	//レールカメラ
	railCamera_->Initialize(Vector3(0, 0, -50), Vector3(0, 0, 0));
	player_->SetParent(&railCamera_->GetWorldTransform());
	
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

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Radian_transform(20.0f);
	//アスペクト比
	// viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	// viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
	// viewProjection_.farZ = 53.0f;

	viewProjection_.Initialize();
	//debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	//debugCamera_->Update();
	railCamera_->Update();

	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();

	player_->Update();
	enemy_->Update();
	skydome_->Update();
	CheckAllCollision();

}

void GameScene::CheckAllCollision() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
	#pragma	region	自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>&e_bullet:enemyBullets) {
		//敵弾の座標
		posB = e_bullet->GetBulletPosition();
		//A,Bの距離
		Vector3 vecPos = lens(posA, posB);
		float dis = length(vecPos);
		//
		float	radius =player_->GetRadius() + e_bullet->GetRadius();
		//判定
		if(dis<=radius) {
			//自キャラのコールバックを呼び出し
			player_->OnCollision();
			//敵弾のコールバックを呼び出し
			e_bullet->OnCollision();
		}
	}
	#pragma	endregion

	#pragma region 自弾と敵キャラの当たり判定
	//敵弾の座標
	posA = enemy_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& p_bullet : playerBullets) {
		//自弾の座標
		posB = p_bullet->GetBulletPosition();
		// A,Bの距離
		Vector3 vecPos = lens(posA, posB);
		float dis = length(vecPos);
		//
		float radius = enemy_->GetRadius() + p_bullet->GetRadius();
		//判定
		if (dis <= radius) {
			//敵キャラのコールバックを呼び出し
			enemy_->OnCollision();
			//自弾のコールバックを呼び出し
			p_bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と敵弾の当たり判定
	//自弾の座標
	for (const std::unique_ptr<PlayerBullet>& p_bullet : playerBullets) {
		posA = p_bullet->GetBulletPosition();
		//自キャラと敵弾全ての当たり判定
		for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
			//敵弾の座標
			posB = e_bullet->GetBulletPosition();
			// A,Bの距離
			Vector3 vecPos = lens(posA, posB);
			float dis = length(vecPos);
			//
			float radius = enemy_->GetRadius() + p_bullet->GetRadius();
			//判定
			if (dis <= radius) {
				//自弾のコールバックを呼び出し
				p_bullet->OnCollision();
				//敵弾のコールバックを呼び出し
				e_bullet->OnCollision();
			}
		}
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
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
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
