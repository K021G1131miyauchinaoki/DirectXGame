#include"EnemyBullet.h"
#include "Mat.h"
#include <assert.h>
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	velocity_ = velocity;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.png");
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数で受け取って初期座標をセット
	worldTransform_.translation_ = position;
}

void EnemyBullet::Update() {
	worldTransform_.translation_ -= velocity_;
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
	//時間経過で消滅
	if (--deathTimer <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}