#include "Enemy.h"
#include "Mat.h"
#include "Player.h"
#include <cassert>


//初期化
void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	debugText_ = DebugText::GetInstance();
	ApproachInitialize();
	worldTransform_.Initialize();
	worldTransform_.translation_ = {4.0f, 3.0f, 20.0f};
}

//更新
void Enemy::Update() {
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach_move();
		break;
	case Phase::Leave:
		Leave_move();

		break;
	}

#pragma region 移動処理
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);
	//行列の転送
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "Enemy:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

#pragma endregion
	Fire();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

//弾発射
void Enemy::Fire() {
	assert(player_);
	if (bullletTime-- < 0) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity;
		
		Vector3 pPos = player_->GetWorldPosition();
		Vector3 ePos = GetWorldPosition();

		Vector3 len = ePos-pPos;
		velocity = normaleize(len);

		len *= kBulletSpeed;
		//弾を生成し、初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

		bullletTime = kFireInterval;
	}
}

//弾
void Enemy::ApproachInitialize() { bullletTime = kFireInterval; }

//行動パターン
void Enemy::Approach_move() {
	//移動
	Vector3 move = {0, 0, -0.2f};
	worldTransform_.translation_ += move;
	//一定の位置に達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::Leave_move() {
	//移動
	Vector3 move = {-0.2f, 0, -0.2f};
	worldTransform_.translation_ += move;
}

//ワールド座標を渡す
Vector3 Enemy::GetWorldPosition() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
//衝突したら
void Enemy::OnCollision() {}

//描画
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
