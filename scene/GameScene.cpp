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

//辺
bool SideHit(float a1, float w1, float a2, float w2) {
	float cl1 = a1 - w1;
	float cr1 = cl1 + (w1 * 2);
	float cl2 = a2 - w2;
	float cr2 = cl2 + (w2 * 2);

	if (cr1 <= cl2) {
		return false;
	}

	if (cr2 <= cl1) {
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

bool PlaneHit2(float a1, float b1, float w1, float h1, float a2, float b2, float w2, float h2,float	c1,float	c2) {
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

	float cf1 = c1 - 0.1f;
	float cb1 = c1 + 0.1f;
	float cf2 = c2 - 0.1f;
	float cb2 = c2 + 0.1f;

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

GameScene::~GameScene() {
	delete player_;
	delete floor_;
	for (size_t i = 0; i < fallNums; i++) {

		delete fall_[i];
	}
	for (size_t i = 0; i < swampNums; i++) {
		delete swamp_[i];
	}
	for (size_t i = 0; i < boxNums; i++) {
		delete box_[i];
	}
	delete goal_;

	for (size_t i = 0; i < 6; i++) {
		delete sprite_[i];
	}

	delete modelSkydome_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	sceneFlag = 0;
	stageFalg = 0;

	textureHandle_[0] = TextureManager::Load("player.png");
	textureHandle_[1] = TextureManager::Load("floor.png");
	textureHandle_[2] = TextureManager::Load("fall.png");
	textureHandle_[3] = TextureManager::Load("swamp.png");
	textureHandle_[4] = TextureManager::Load("box.png");
	textureHandle_[5] = TextureManager::Load("goal.png");

	gameHandle[0] = TextureManager::Load("start.png");
	gameHandle[1] = TextureManager::Load("over.png");
	gameHandle[2] = TextureManager::Load("stageclear.png");
	gameHandle[3] = TextureManager::Load("clear.png");

	key = TextureManager::Load("key.png");
	tutorial = TextureManager::Load("next.png");

	gameBgm[0] = audio_->LoadWave("/bgm/start.mp3");
	gameBgm[1] = audio_->LoadWave("/bgm/play.mp3");
	gameBgm[2] = audio_->LoadWave("/bgm/over.mp3");
	gameBgm[3] = audio_->LoadWave("/bgm/clear.mp3");

	

	//スプライト
	for (size_t i = 0; i < 4; i++) {
		sprite_[i] = Sprite::Create(gameHandle[i], {0, 0});
	}
	sprite_[4] = Sprite::Create(key, {1080, 0});
	sprite_[5] = Sprite::Create(tutorial, {720, 0});

	bgmFlag = 0;

	//モデル
	model_ = Model::Create();
	//プレイヤー
	player_ = new Player();
	player_->Initialization(model_, textureHandle_[0]);
	
	//床
	scaleFloor = {5.0f, 1.0f, 100.0f};
	floor_ = new Floor();
	floor_->Initialization(model_, textureHandle_[1]);

	//落下
	for (size_t i = 0; i < fallNums; i++) {
		fall_[i] = new Fall;
		fall_[i]->Initialization(model_, textureHandle_[2]);
	}

	//沼
	scaleSwamp = {4.0f, 6.0f, 6.0f};
	for (size_t i = 0; i < swampNums; i++) {
		swamp_[i] = new Swamp;
		swamp_[i]->Initialization(model_, textureHandle_[3]);
	}
	//箱
	
	scaleBox = {3.0f, 5.0f, 4.0f};
	for (size_t i = 0; i < boxNums; i++) {
		box_[i] = new MoveBox;
		box_[i]->Initialization(model_, textureHandle_[4]);
	}

	//カメラ
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	
	//ゴール
	goal_ = new Goal;
	goal_->Initialization(model_, textureHandle_[5]);

	//天球
	// 3Dモデル生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	//カメラ垂直方向視野角を設定
	//viewProjection_.fovAngleY = Radian_transform(20.0f);
	//アスペクト比
	// viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	// viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
	// viewProjection_.farZ = 53.0f;

	viewProjection_.Initialize();

	//軸方向表示の表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する
	// PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	
	//半径
	r = 1.0f;
}

void GameScene::Update() { 
	#pragma region プレイ
	//タイトル
	if (sceneFlag==0) {
		#pragma region 初期化
		//プレイヤー
		player_->State();

		//床
		floor_->State( scaleFloor);

		//落下
		for (int i = 0; i < fallNums; i++) {
			fall_[i]->State(transFall[i]);
		}

		//カメラ
		camera_->State(Vector3(0, 0, -100), Vector3(0, 0, 0));
		stageFalg = 0;

		//ゴール
		goalZ = 50;
		goal_->State(goalZ);
		#pragma	endregion
		if (input_->PushKey(DIK_UP)) {
			sceneFlag = 1;
		}


	} else if (sceneFlag == 1) {
		//プレイヤー
		player_->Update();
		//床
		floor_->Update();
		//天球
		skydome_->Update();

		//チュートリアル
		if (stageFalg == 0) {
			CheckAllCollision0();
			camera_->Update();
			viewProjection_.matView = camera_->GetViewProjection().matView;
			viewProjection_.matProjection = camera_->GetViewProjection().matProjection;

			viewProjection_.TransferMatrix();
			if (input_->PushKey(DIK_G)) {
				stageFalg = 1;
				//プレイヤー
				player_->State();
			}
		}
		//ステージ
		//1
		else if (stageFalg == 1) {

			CheckAllCollision1();
			camera_->Update();
			viewProjection_.matView = camera_->GetViewProjection().matView;
			viewProjection_.matProjection = camera_->GetViewProjection().matProjection;

			viewProjection_.TransferMatrix();
			//落下物
			for (size_t i = 0; i < fallNums; i++) {
				fall_[i]->SetPlayer(player_->GetWorldPosition().z);
				fall_[i]->Update(transFall[i]);
			}

			//ゴール
			goal_->Update();
		}
		//2
		else if (stageFalg == 2) {
			CheckAllCollision2();
			camera_->Update();
			viewProjection_.matView = camera_->GetViewProjection().matView;
			viewProjection_.matProjection = camera_->GetViewProjection().matProjection;

			viewProjection_.TransferMatrix();

			//落下物
			for (size_t i = 0; i < fallNum2; i++) {
				fall_[i]->SetPlayer(player_->GetWorldPosition().z);
				fall_[i]->Update(transFall2[i]);
			}
			//沼
			//swamp_[0]->Update();
			//箱
			for (size_t i = 0; i < boxNum2; i++) {
				box_[i]->Update();
			}

			//ゴール
			goal_->Update();
		}
		// 3
		else if (stageFalg == 3) {
			CheckAllCollision3();
			camera_->Update();
			viewProjection_.matView = camera_->GetViewProjection().matView;
			viewProjection_.matProjection = camera_->GetViewProjection().matProjection;

			viewProjection_.TransferMatrix();

			//落下物
			for (size_t i = 0; i < fallNum3; i++) {
				fall_[i]->SetPlayer(player_->GetWorldPosition().z);
				fall_[i]->Update(transFall3[i]);
			}
			//沼
			for (int i = 0; i < swampNum3; i++) {
				swamp_[i]->Update();
			}
			//箱
			for (size_t i = 0; i < boxNum3; i++) {
				box_[i]->Update();
			}

			//ゴール
			goal_->Update();
		}

		/// フラグが立ったら
		if (!player_->GetFlag()) {
			sceneFlag = 2;
			bgmFlag = 2;
		}
		if (goal_->GetFlag()) {
			bgmFlag = 3;
			sceneFlag = 3;
		}
	}
	//ゲームオーバー 2
	//ゲームクリア	 3
	if (sceneFlag == 2 || sceneFlag == 3) {
		//Fでタイトル
		if (input_->PushKey(DIK_F)) {
			sceneFlag = 0;
		}
		//Rでリトライ
		if (input_->PushKey(DIK_R)) {
			sceneFlag = 1;
			bgmFlag = 1;
			//ステージ1
			if (stageFalg==1) {
				//プレイヤー
				player_->State();

				//床
				floor_->State(scaleFloor);

				//落下
				for (int i = 0; i < fallNums; i++) {
					fall_[i]->State(transFall[i]);
				}

				//カメラ
				camera_->State(Vector3(0, 0, -100), Vector3(0, 0, 0));
				stageFalg = 1;

				//ゴール
				goal_->State(goalZ);
			}
			//ステージ2
			if (stageFalg == 2) {
				//プレイヤー
				player_->State();

				//床
				floor_->State(scaleFloor);

				//落下
				for (int i = 0; i < fallNum2; i++) {
					fall_[i]->State(transFall2[i]);
				}

				//沼
				//swamp_[0]->State(scaleSwamp, transSwamp);

				//箱
				for (size_t i = 0; i < boxNum2; i++) {
					box_[i]->State(scaleBox, transBox2[i]);
				}
				//カメラ
				camera_->State(Vector3(0, 0, -100), Vector3(0, 0, 0));
				stageFalg = 2;

				//ゴール
				goal_->State(goalZ);
			}
			//ステージ3
			if (stageFalg == 3) {
				//プレイヤー
				player_->State();

				//床
				floor_->State(scaleFloor);

				//落下
				for (int i = 0; i < fallNum3; i++) {
					fall_[i]->State(transFall3[i]);
				}

				//沼
				for (int i = 0; i < swampNum3; i++) {
					swamp_[i]->State(scaleSwamp, transSwamp3[i]);
				}
				//箱
				for (size_t i = 0; i < boxNum3; i++) {
					box_[i]->State(scaleBox, transBox3[i]);
				}
				//カメラ
				camera_->State(Vector3(0, 0, -100), Vector3(0, 0, 0));

				//ゴール
				goal_->State(goalZ);
			}
			
		}
		//G
		if (input_->PushKey(DIK_G) && sceneFlag == 3&&stageFalg!=3) {
				sceneFlag = 1;
				bgmFlag = 1;
				stageFalg += 1;
				//ステージ2
				if (stageFalg == 2) {
				//プレイヤー
				player_->State();

				//床
				floor_->State(scaleFloor);

				//落下
				for (int i = 0; i < fallNum2; i++) {
					fall_[i]->State(transFall2[i]);
				}

				//箱
				for (size_t i = 0; i < boxNum2; i++) {
					box_[i]->State(scaleBox, transBox2[i]);
				}
				//カメラ
				camera_->State(Vector3(0, 0, -100), Vector3(0, 0, 0));

				//ゴール
				goalZ = 10;
				goal_->State(goalZ);
				}
			    //ステージ3
			    if (stageFalg == 3) {
				    //プレイヤー
				    player_->State();

				    //床
				    floor_->State(scaleFloor);

				    //落下
				    for (int i = 0; i < fallNum3; i++) {
					    fall_[i]->State(transFall3[i]);
				    }

				    //沼
				    for (int i = 0; i < swampNum3; i++) {
					    swamp_[i]->State(scaleSwamp, transSwamp3[i]);
				    }
				    //箱
				    for (size_t i = 0; i < boxNum3; i++) {
					    box_[i]->State(scaleBox, transBox3[i]);
				    }
				    //カメラ
				    camera_->State(Vector3(0, 0, -100), Vector3(0, 0, 0));

				    //ゴール
				    goalZ = 10;
				    goal_->State(goalZ);
			    }
		}
	}
	#pragma	endregion

	 #pragma region bgm
	switch (sceneFlag) {
	case 0:
		if (bgmFlag == 0) {
			audio_->StopWave(soundDrain);
			soundDrain = audio_->PlayWave(gameBgm[0], true);
			bgmFlag = 1;
		}
		break;
	case 1:
		if (bgmFlag == 1) {
			audio_->StopWave(soundDrain);
			soundDrain = audio_->PlayWave(gameBgm[1], true);
			bgmFlag = 4;
		}
		break;
	case 2:
		if (bgmFlag == 2) {
			audio_->StopWave(soundDrain);
			soundDrain = audio_->PlayWave(gameBgm[2], true);
			bgmFlag = 0;
		}
		break;
	case 3:
		if (bgmFlag == 3) {
			audio_->StopWave(soundDrain);
			soundDrain = audio_->PlayWave(gameBgm[3], true);
			bgmFlag = 0;
		}
		break;
	}
	#pragma	endregion

	camera_->GetFlag(stageFalg);
	debugText_->SetPos(0, 0);
	debugText_->Printf("%d", sceneFlag);
	debugText_->SetPos(0, 20);
	debugText_->Printf("%d", stageFalg);
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
	if (sceneFlag==1) {
		player_->Draw(viewProjection_);
		floor_->Draw(viewProjection_);
		skydome_->Draw(viewProjection_);
		//ステージ1
		if (stageFalg == 1) {
			//落下物
			for (size_t i = 0; i < fallNums; i++) {
				fall_[i]->Draw(viewProjection_);
			}
			//ゴール
			goal_->Draw(viewProjection_);
		}
		//ステージ2
		if (stageFalg == 2) {
			for (size_t i = 0; i < fallNum2; i++) {
				fall_[i]->Draw(viewProjection_);
			}
			//swamp_[0]->Draw(viewProjection_);
			//箱
			for (size_t i = 0; i < boxNum2; i++) {
				box_[i]->Draw(viewProjection_);
			}
			//ゴール
			goal_->Draw(viewProjection_);
		}
		//ステージ3
		if (stageFalg == 3) {
			//落下
			for (int i = 0; i < fallNum3; i++) {
				fall_[i]->Draw(viewProjection_);
			}

			//沼
			for (int i = 0; i < swampNum3; i++) {
				swamp_[i]->Draw(viewProjection_);
			}
			//箱
			for (size_t i = 0; i < boxNum3; i++) {
				box_[i]->Draw(viewProjection_);
			}
			//ゴール
			goal_->Draw(viewProjection_);
		}
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//
	if (sceneFlag==0) {
		sprite_[0]->Draw();
	}
	//
	if (sceneFlag==1) {
		if (stageFalg==0) {
			sprite_[5]->Draw();
		}
		sprite_[4]->Draw();
	}
	//
	if (sceneFlag ==2) {
		sprite_[1]->Draw();
	}
	//
	if (sceneFlag ==3) {
		if (stageFalg!=3) {
			sprite_[2]->Draw();
		} else {
			sprite_[3]->Draw();
		}
	}
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//
void GameScene::CheckAllCollision0() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	posA = player_->GetWorldPosition();
	posB = floor_->GetWorldPosition();
	#pragma region 自キャラと床の当たり判定
	//ジャンプ用
	{
		if (CheckHit(
		      posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleFloor.x, scaleFloor.y,
		      scaleFloor.z)) {
			if (
			  PlaneHit(
			    posA.x, posA.y, r, r, posB.x, posB.y, scaleFloor.x, scaleFloor.y,
			    posB.z + scaleFloor.z, posA.z - r) ||
			  PlaneHit(
			    posA.x, posA.y, r, r, posB.x, posB.y, scaleFloor.x, scaleFloor.y,
			    posB.z - scaleFloor.z, posA.z + r) ||
			  PlaneHit(
			    posA.z, posA.y, r, r, posB.z, posB.y, scaleFloor.z, scaleFloor.y,
			    posB.x - scaleFloor.x, posA.x + r) ||
			  PlaneHit(
			    posA.z, posA.y, r, r, posB.z, posB.y, scaleFloor.z, scaleFloor.y,
			    posB.x + scaleFloor.x, posA.x - r)) {
				player_->SideCollision();
			} else if (PlaneHit(
			             posA.x, posA.z, r, r, posB.x, posB.z, scaleFloor.x, scaleFloor.z,
			             posB.y + scaleFloor.y, posA.y - r)) {
				player_->UpCollision();
			} else if (PlaneHit(
			             posA.x, posA.z, r, r, posB.x, posB.z, scaleFloor.x, scaleFloor.z,
			             posB.y - scaleFloor.y, posA.y + r)) {
				player_->DownCollision();
			}
		}
		//床に当たっていないとき
		 else if (!CheckHit(
		           posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleFloor.x,
		           scaleFloor.y + 1.0f, scaleFloor.z)) {
			player_->OffCollision();
		}
	}
#pragma endregion
}

void GameScene::CheckAllCollision1() {
	//判定対象AとBの座標
	Vector3 posA, posB,posC;

	posA = player_->GetWorldPosition();
	posB = floor_->GetWorldPosition();
	#pragma region 自キャラと床の当たり判定
	//ジャンプ用
	{
		if (CheckHit(
		      posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleFloor.x, scaleFloor.y,
		      scaleFloor.z)) {
			if (
			  PlaneHit(
			    posA.x, posA.y, r, r, posB.x, posB.y, scaleFloor.x, scaleFloor.y,
			    posB.z + scaleFloor.z, posA.z - r) ||
			  PlaneHit(
			    posA.x, posA.y, r, r, posB.x, posB.y, scaleFloor.x, scaleFloor.y,
			    posB.z - scaleFloor.z, posA.z + r) ||
			  PlaneHit(
			    posA.z, posA.y, r, r, posB.z, posB.y, scaleFloor.z, scaleFloor.y,
			    posB.x - scaleFloor.x, posA.x + r) ||
			  PlaneHit(
			    posA.z, posA.y, r, r, posB.z, posB.y, scaleFloor.z, scaleFloor.y,
			    posB.x + scaleFloor.x, posA.x - r)) {
				player_->SideCollision();
			} else if (PlaneHit(
			             posA.x, posA.z, r, r, posB.x, posB.z, scaleFloor.x, scaleFloor.z,
			             posB.y + scaleFloor.y, posA.y - r)) {
				player_->UpCollision();
			} else if (PlaneHit(
			             posA.x, posA.z, r, r, posB.x, posB.z, scaleFloor.x, scaleFloor.z,
			             posB.y - scaleFloor.y, posA.y + r)) {
				player_->DownCollision();
			}
		}
		//床に当たっていないとき
		//else if (!CheckHit(
		//           posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleFloor.x,
		//           scaleFloor.y + 1.0f, scaleFloor.z)) {
		//	player_->OffCollision();
		//}
	}
	#pragma endregion
	#pragma region 自キャラと落下物の当たり判定
	{
		for (size_t i = 0; i < fallNums; i++) {
			posC = fall_[i]->GetWorldPosition();
			if (CheckHit(posA.x, posA.y, posA.z, r, r, r, posC.x, posC.y, posC.z, r, r, r)) {
				player_->OnCollision();
				fall_[i]->OnCollision();
			}
		}
	}
	#pragma endregion
	#pragma region 自キャラとゴールの当たり判定

	{
		posC = goal_->GetWorldPosition();
		if (posA.z>posC.z) {
			goal_->OnCollision();
		}
	}
	#pragma endregion
}

void GameScene::CheckAllCollision2() {
	//判定対象AとBの座標
	Vector3 posA, posB, posC;

	posA = player_->GetWorldPosition();
	posB = floor_->GetWorldPosition();
	#pragma region 自キャラと床の当たり判定
	//ジャンプ用
	{
		if (CheckHit(
		      posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleFloor.x, scaleFloor.y,
		      scaleFloor.z)) {
			if (
			  PlaneHit(
			    posA.x, posA.y, r, r, posB.x, posB.y, scaleFloor.x, scaleFloor.y,
			    posB.z + scaleFloor.z, posA.z - r) ||
			  PlaneHit(
			    posA.x, posA.y, r, r, posB.x, posB.y, scaleFloor.x, scaleFloor.y,
			    posB.z - scaleFloor.z, posA.z + r) ||
			  PlaneHit(
			    posA.z, posA.y, r, r, posB.z, posB.y, scaleFloor.z, scaleFloor.y,
			    posB.x - scaleFloor.x, posA.x + r) ||
			  PlaneHit(
			    posA.z, posA.y, r, r, posB.z, posB.y, scaleFloor.z, scaleFloor.y,
			    posB.x + scaleFloor.x, posA.x - r)) {
				player_->SideCollision();
			} else if (PlaneHit(
			             posA.x, posA.z, r, r, posB.x, posB.z, scaleFloor.x, scaleFloor.z,
			             posB.y + scaleFloor.y, posA.y - r)) {
				player_->UpCollision();
			} else if (PlaneHit(
			             posA.x, posA.z, r, r, posB.x, posB.z, scaleFloor.x, scaleFloor.z,
			             posB.y - scaleFloor.y, posA.y + r)) {
				player_->DownCollision();
			}
		}
		//床に当たっていないとき
		// else if (!CheckHit(
		//           posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleFloor.x,
		//           scaleFloor.y + 1.0f, scaleFloor.z)) {
		//	player_->OffCollision();
		//}
	}
#pragma endregion
	#pragma region 自キャラと落下物の当たり判定
	{
		for (size_t i = 0; i < fallNum2; i++) {
			posC = fall_[i]->GetWorldPosition();
			if (CheckHit(posA.x, posA.y, posA.z, r, r, r, posC.x, posC.y, posC.z, r, r, r)) {
				player_->OnCollision();
				fall_[i]->OnCollision();
			}
		}
	}
#pragma endregion
	#pragma region 自キャラと箱の当たり判定

	{
		for (size_t i = 0; i < boxNum2; i++) {
			posC = box_[i]->GetWorldPosition();
			if (CheckHit(
			      posA.x, posA.y, posA.z, r, r, r, posC.x, posC.y, posC.z, scaleBox.x, scaleBox.y,
			      scaleBox.z)) {
				if (PlaneHit(
				      posA.z, posA.y, r, r, posC.z, posC.y, scaleBox.z, scaleBox.y,
				      posC.x + scaleBox.x, posA.x - r)) {
					player_->SideCollision3();

				} else if (PlaneHit(
				             posA.z, posA.y, r, r, posC.z, posC.y, scaleBox.z, scaleBox.y,
				             posC.x - scaleBox.x, posA.x + r)) {
					player_->SideCollision4();

				} else if (
				  PlaneHit(
				    posA.x, posA.y, r, r, posC.x, posC.y, scaleBox.x, scaleBox.y,
				    posC.z + scaleBox.z, posA.z - r) ||
				  PlaneHit(
				    posA.x, posA.y, r, r, posC.x, posC.y, scaleBox.x, scaleBox.y,
				    posC.z - scaleBox.z, posA.z + r)) {
					player_->SideCollision2();
				}
			}
			//床に当たっていないとき
			else if (
			  !CheckHit(
			    posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleBox.x,
			    scaleBox.y + 1.0f, scaleBox.z) &&
			  !CheckHit(
			    posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleFloor.x,
			    scaleFloor.y + 1.0f, scaleFloor.z)) {
				player_->OffCollision();
			}
		}
	}
#pragma endregion
	#pragma region 自キャラとゴールの当たり判定

	{
		posC = goal_->GetWorldPosition();
		if (posA.z > posC.z) {
			goal_->OnCollision();
		}
	}
#pragma endregion
}


 void GameScene::CheckAllCollision3() {
	//判定対象AとBの座標
	Vector3 posA, posB, posC;

	posA = player_->GetWorldPosition();
	posB = floor_->GetWorldPosition();
	#pragma region 自キャラと床の当たり判定
	//ジャンプ用
	{
		if (CheckHit(
		      posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleFloor.x, scaleFloor.y,
		      scaleFloor.z)) {
			if (
			  PlaneHit(
			    posA.x, posA.y, r, r, posB.x, posB.y, scaleFloor.x, scaleFloor.y,
			    posB.z + scaleFloor.z, posA.z - r) ||
			  PlaneHit(
			    posA.x, posA.y, r, r, posB.x, posB.y, scaleFloor.x, scaleFloor.y,
			    posB.z - scaleFloor.z, posA.z + r) ||
			  PlaneHit(
			    posA.z, posA.y, r, r, posB.z, posB.y, scaleFloor.z, scaleFloor.y,
			    posB.x - scaleFloor.x, posA.x + r) ||
			  PlaneHit(
			    posA.z, posA.y, r, r, posB.z, posB.y, scaleFloor.z, scaleFloor.y,
			    posB.x + scaleFloor.x, posA.x - r)) {
				player_->SideCollision();
			} else if (PlaneHit(
			             posA.x, posA.z, r, r, posB.x, posB.z, scaleFloor.x, scaleFloor.z,
			             posB.y + scaleFloor.y, posA.y - r)) {
				player_->UpCollision();
			} else if (PlaneHit(
			             posA.x, posA.z, r, r, posB.x, posB.z, scaleFloor.x, scaleFloor.z,
			             posB.y - scaleFloor.y, posA.y + r)) {
				player_->DownCollision();
			}
		}
		//床に当たっていないとき
		// else if (!CheckHit(
		//           posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleFloor.x,
		//           scaleFloor.y + 1.0f, scaleFloor.z)) {
		//	player_->OffCollision();
		//}
	}
#pragma endregion
	#pragma region 自キャラと落下物の当たり判定
	{
		for (size_t i = 0; i < fallNum3; i++) {
			posC = fall_[i]->GetWorldPosition();
			if (CheckHit(posA.x, posA.y, posA.z, r, r, r, posC.x, posC.y, posC.z, r, r, r)) {

				debugText_->SetPos(60, 0);
				debugText_->Printf("5");
				debugText_->SetPos(20, 0);
				debugText_->Printf("hit");
				player_->OnCollision();
				fall_[i]->OnCollision();
			}
		}
	}
#pragma endregion
	#pragma region 自キャラと沼の当たり判定
	{
		for (size_t i = 0; i < swampNum3; i++) {

			posC = swamp_[i]->GetWorldPosition();
			if (SideHit(posA.z, r, posC.z, scaleSwamp.z)) {
				player_->SwampCollision();
			}
		}
	}
#pragma endregion
	#pragma region 自キャラと箱の当たり判定

	{
		for (size_t i = 0; i < boxNum3; i++) {

			posC = box_[i]->GetWorldPosition();
			if (CheckHit(
			      posA.x, posA.y, posA.z, r, r, r, posC.x, posC.y, posC.z, scaleBox.x, scaleBox.y,
			      scaleBox.z)) {
				if (PlaneHit(
				      posA.z, posA.y, r, r, posC.z, posC.y, scaleBox.z, scaleBox.y,
				      posC.x + scaleBox.x, posA.x - r)) {
					player_->SideCollision3();

				} else if (PlaneHit(
				             posA.z, posA.y, r, r, posC.z, posC.y, scaleBox.z, scaleBox.y,
				             posC.x - scaleBox.x, posA.x + r)) {
					player_->SideCollision4();

				} else if (
				  PlaneHit(
				    posA.x, posA.y, r, r, posC.x, posC.y, scaleBox.x, scaleBox.y,
				    posC.z + scaleBox.z, posA.z - r) ||
				  PlaneHit(
				    posA.x, posA.y, r, r, posC.x, posC.y, scaleBox.x, scaleBox.y,
				    posC.z - scaleBox.z, posA.z + r)) {
					player_->SideCollision2();
					
				}
			}
			//床に当たっていないとき
			else if (
			  !CheckHit(
			    posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleBox.x,
			    scaleBox.y + 1.0f, scaleBox.z) &&
			  !CheckHit(
			    posA.x, posA.y, posA.z, r, r, r, posB.x, posB.y, posB.z, scaleFloor.x,
			    scaleFloor.y + 1.0f, scaleFloor.z)) {
				player_->OffCollision();
			}
		}
	}
#pragma endregion
	#pragma region 自キャラとゴールの当たり判定

	{
		posC = goal_->GetWorldPosition();
		if (posA.z > posC.z) {
			goal_->OnCollision();
		}
	}
#pragma endregion
}