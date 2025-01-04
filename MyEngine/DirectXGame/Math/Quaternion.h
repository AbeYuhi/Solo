#pragma once
#include <math.h>
#include "Vector3.h"
#include "Matrix4x4.h"

/// <summary>
/// Quaternion.h
/// Quaternion構造体のデータファイル
/// </summary>

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

Quaternion operator*(const Quaternion& num1, const float num2);
Quaternion operator*(const float num1, const Quaternion& num2);
Quaternion operator+(const Quaternion& num1, const Quaternion& num2);
bool operator==(const Quaternion& num1, const Quaternion& num2);

/// <summary>
/// 積を求める関数
/// </summary>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

/// <summary>
/// 単位を求める関数
/// </summary>
/// <returns></returns>
Quaternion IdentityQuaternion();

/// <summary>
/// 共役を求める関数
/// </summary>
/// <param name="quaternion"></param>
/// <returns></returns>
Quaternion Conjugate(const Quaternion& quaternion);

/// <summary>
/// ノルムを求める関数
/// </summary>
/// <param name="quaternion"></param>
/// <returns></returns>
float Norm(const Quaternion& quaternion);

/// <summary>
/// 正規化する関数
/// </summary>
/// <param name="quaternion"></param>
/// <returns></returns>
Quaternion Normalize(const Quaternion& quaternion);

/// <summary>
/// 逆を求める関数
/// </summary>
/// <param name="quaternion"></param>
/// <returns></returns>
Quaternion Inverse(const Quaternion& quaternion);

/// <summary>
/// 内積を求める関数
/// </summary>
/// <param name="q0"></param>
/// <param name="q1"></param>
/// <returns></returns>
float Dot(const Quaternion& q0, const Quaternion& q1);

/// <summary>
/// 任意回転軸を表すQuaternionを求める関数
/// </summary>
/// <param name="axis"></param>
/// <param name="angle"></param>
/// <returns></returns>
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

//ベクトルをQuaternionで回転させた結果のベクトル
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

/// <summary>
/// 回転行列を求める関数
/// </summary>
/// <param name="quaternion"></param>
/// <returns></returns>
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

/// <summary>
/// 線形補間する関数
/// </summary>
/// <param name="q0"></param>
/// <param name="q1"></param>
/// <param name="t"></param>
/// <returns></returns>
Quaternion Slerp(Quaternion q0, Quaternion q1, float t);
