#include"RailCamera.h"
#include"Mat.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	//ワールド行列
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	//ビュープロジェクション初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update() { 
	//移動（ベクトルを加算）
	worldTransform_.translation_ += Vector3(0, 0, 0.1f);
	//ワールドトランスフォームの更新
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ *= Mat_size(worldTransform_);
	worldTransform_.matWorld_ *= Mat_rot(worldTransform_);
	worldTransform_.matWorld_ *= Mat_move(worldTransform_);
	//ワールド行列の平行移動成分を取得
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	//ワールドトランスフォームの座標を加算
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = Vector3Transform(forward, worldTransform_.matWorld_);
	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target -= viewProjection_.eye += forward; 
	
	//ワールドトランスフォームの角度を加算
	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映
	up = Vector3Transform(up, worldTransform_.matWorld_);
	viewProjection_.up += up;

	//ビュープロジェクションの更新
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(20, 110);
	debugText_->Printf(
	  "RailCamera Pos:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}