#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>
#include <cmath>

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);
Vector3 Multiply(const Vector3& v1, const Vector3& v2);
Vector3 Multiply(const Vector3& v, const Matrix4x4& M);
// 長さ
float Length(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector3& v);
// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

//拡縮行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
    //回転行列
Matrix4x4 MakeRotateMatrix(const Vector3& rotate);
// 1.x軸の回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// 2.y軸の回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// 3.z軸の回転行列
Matrix4x4 MakeRotateZMatrix(float radian);
//移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
//ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
// 3次元アフィン変換
Matrix4x4 MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
// 投資投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspecRatio, float nearClip, float farClip);
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

//円の衝突判定
bool circleCollision(Vector3 v1, Vector3 v2, float radiusV1, float radiusV2);