#include "Player.h"
#include "Mat.h"
#include "RailCamera.h"
#include <cassert>

//初期化
void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {0, 0, 24};
	worldTransform_.Initialize();
	// worldTransform_.parent_ = &railCamera_->GetWorldMatrix();
}

//更新
void Player::Update() {
	// assert(railCamera_);
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });
	//回転
	Rotate();
	//移動
	Vector3 move = {0, 0, 0};

	const float speed = 0.2f;
#pragma region 移動処理
	if (input_->PushKey(DIK_UP)) {
		move = {0, speed, 0};
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, -speed, 0};
	} else if (input_->PushKey(DIK_LEFT)) {
		move = {-speed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {speed, 0, 0};
	}

	worldTransform_.translation_ += move;
	//上限、下限の設定
	const float kMoveLimitX = 14.0f;
	const float kMoveLimitY = 8.0f;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	//行列の転送
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

#pragma endregion
	Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

//攻撃
void Player::Attack() {
	if (bullletTime-- < 0 && input_->PushKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = Vector3Transform(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.matWorld_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

		//タイマーリセット
		bullletTime = kFireInterval;
	}
}
//回転
void Player::Rotate() {

	const float kRot = 0.05f;
	Vector3 Rot = {0, 0, 0};
	if (input_->PushKey(DIK_U)) {
		Rot = {0, kRot, 0};
	} else if (input_->PushKey(DIK_I)) {
		Rot = {0, -kRot, 0};
	}
	worldTransform_.rotation_ += Rot;
}

//ワールド座標を渡す
Vector3 Player::GetWorldPosition() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	//worldTransform_.matWorld_.m;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
//衝突したら
void Player::OnCollision() {}

//描画
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}