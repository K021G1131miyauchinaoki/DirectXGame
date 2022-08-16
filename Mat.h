#include "DirectXCommon.h"
#include "WorldTransform.h"
#pragma once

Matrix4 matIdentity();
//スケーリング
Matrix4 Mat_size(WorldTransform& w);
//平行移動
Matrix4 Mat_move(WorldTransform& w);
//回転
Matrix4 Mat_rot(WorldTransform& w);
//合成
Matrix4 Mat(WorldTransform w);

Vector3 Vector3Transform(Vector3 velocity, Matrix4 amount);

Vector3 lens(Vector3 vec1, Vector3 vec2);
float length(Vector3 vec);
Vector3& normaleize(Vector3 vec_);