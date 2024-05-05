#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>
#include <cmath>

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 1.x軸の回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// 2.y軸の回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// 3.z軸の回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
//ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
    // 3次元アフィン変換
Matrix4x4 MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);