#include"PayerBullet.h"
#include"Mat.h"
#include<assert.h>
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) { 
	//NULLポインタチェック
	assert(model);
	model_ = model;
	velocity_ = velocity;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数で受け取って初期座標をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {
	worldTransform_.translation_ += velocity_;
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}