#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"	
#pragma once
class PlayerBullet {
  public:
	void Initialize(Model* model, const Vector3& position, const Vector3&	veclocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
  private:
	WorldTransform worldTransform_;
	Model* model_=nullptr;
	uint32_t textureHandle_ = 0u;	
	//‘¬“x
	Vector3 velocity_;
};