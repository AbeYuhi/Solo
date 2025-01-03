#pragma once
#include <math.h>
#include <algorithm>
#include <vector>
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include "Quaternion.h"
#include "AABB.h"
#include "OBB.h"
#include "Sphere.h"

/// <summary>
/// Math.h
/// 数学に関する関数をまとめているファイル
/// </summary>

struct EulerTransformData {
	Vector3 scale_;
	Vector3 rotate_;
	Vector3 translate_;

	void Initialize();
};

struct QuaternionTransformData {
	Vector3 scale_;
	Quaternion rotate_;
	Vector3 translate_;

	void Initialize();
};

static const int kColumnWidth = 60;
static const int kRowHeight = 20;

#pragma region Vectror3
Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(float scalar, const Vector3& v);

float Dot(const Vector3& v1, const Vector3& v2);

float Length(const Vector3& v);

Vector3 Normalize(const Vector3& v1);

Vector3 Cross(const Vector3& v1, const Vector3& v2);

bool IsFront(const Vector3& v1, const Vector3 obj[3]);

Vector3 Project(const Vector3& v1, const Vector3& v2);

Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Vector3 Perpendicular(const Vector3& v);

Vector3 GetPivotBottomPosition(const Vector3& basePosition, float height);

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

#pragma endregion

#pragma region Matrix4x4

Matrix4x4 operator*(const Matrix4x4& num1, const Matrix4x4& num2);
Matrix4x4 operator*=(Matrix4x4& num1, const Matrix4x4& num2);
bool operator==(const Matrix4x4& num1, const Matrix4x4& num2);

/// <summary>
/// 4x4行列の加算
/// </summary>
/// <param name="matrix1">行列1</param>
/// <param name="matrix2">行列2</param>
/// <returns></returns>
Matrix4x4 Add(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

/// <summary>
/// 4x4行列の減算
/// </summary>
/// <param name="matrix1">行列1</param>
/// <param name="matrix2">行列2</param>
/// <returns></returns>
Matrix4x4 Subtract(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

/// <summary>
/// 4x4行列の積
/// </summary>
/// <param name="matrix1">行列1</param>
/// <param name="matrix2">行列2</param>
/// <returns></returns>
Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

/// <summary>
/// 4x4行列の行列式
/// </summary>
/// <param name="matrix">行列式を求めたい行列</param>
/// <returns></returns>
float Det(const Matrix4x4& matrix);

/// <summary>
/// 4x4行列の逆行列
/// </summary>
/// <param name="matrix">逆行列にしたい行列</param>
/// <returns></returns>
Matrix4x4 Inverse(const Matrix4x4& matrix);

/// <summary>
/// 4x4行列の転置行列
/// </summary>
/// <param name="matrix">転置させたい行列</param>
/// <returns></returns>
Matrix4x4 Transpose(const Matrix4x4& matrix);

/// <summary>
/// 4x4単位行列の作成
/// </summary>
/// <returns></returns>
Matrix4x4 MakeIdentity4x4();

Matrix4x4 MakeRotateXMatrix(float radian);

Matrix4x4 MakeRotateYMatrix(float radian);

Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeRotateXYZMatrix(const Matrix4x4& matrixX, const Matrix4x4& matrixY, const Matrix4x4& matrixZ);

Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

Vector3 ExtractEulerAnglesFromMatrixXZY(const Matrix3x3& rotationMatrix);

Vector3 ExtractEulerAnglesFromMatrixXZY(const Matrix4x4& matrix);

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakeAffineMatrix(const EulerTransformData& transformData);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

Matrix4x4 MakeAffineMatrix(const QuaternionTransformData& transformData);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

bool IsCollision(const Vector3& point, const AABB& aabb);

#pragma endregion

#pragma region OBB

bool IsCollision(const OBB& obb, const Sphere& sphere);

bool IsCollision(const Sphere& sphere, const OBB& obb);

bool OverlapOnAxis(const OBB& obb1, const OBB& obb2, const Vector3& axis);

bool IsCollision(const OBB& obb1, const OBB& obb2);

bool IsCollision(const OBB& obb, const AABB& aabb);

bool IsCollision(const AABB& aabb, const OBB& obb);

#pragma endregion

#pragma region Sphere

bool IsCollision(const Sphere& s1, const Sphere& s2);

bool IsCollision(const Sphere& sphere, const AABB& aabb);

bool IsCollision(const AABB& aabb, const Sphere& sphere);

#pragma endregion

void ControlMinMax(AABB& a);

float Clamp(float value, float min, float max);

bool IsCollision(const AABB& a, const AABB& b);

bool IsCollision(const AABB& aabb, const Vector3& point);

Vector3 CalculateNormal(const AABB& a, const AABB& b);
Vector3 CalculateNormal(const AABB& a, const OBB& b);
Vector3 CalculateNormal(const AABB& a, const Sphere& b);

Vector3 CalculateNormal(const OBB& a, const OBB& b);
Vector3 CalculateNormal(const OBB& a, const AABB& b);
Vector3 CalculateNormal(const OBB& a, const Sphere& b);

Vector3 CalculateNormal(const Sphere& a, const Sphere& b);
Vector3 CalculateNormal(const Sphere& a, const AABB& b);
Vector3 CalculateNormal(const Sphere& a, const OBB& b);

Vector3 ClosestPointOnOBB(const OBB& obb, const Vector3& point);

Vector3 GetClosestPointOnOBB(const AABB& aabb0, const Vector3& velocity, const AABB& aabb1);
Vector3 GetClosestPointOnOBB(const AABB& aabb, const Vector3& velocity, const OBB& obb);
Vector3 GetClosestPointOnOBB(const AABB& aabb, const Vector3& velocity, const Sphere& sphere);

Vector3 GetClosestPointOnOBB(const OBB& obb, const Vector3& velocity, const AABB& aabb);
Vector3 GetClosestPointOnOBB(const OBB& obb0, const Vector3& velocity, const OBB& obb1);
Vector3 GetClosestPointOnOBB(const OBB& obb, const Vector3& velocity, const Sphere& sphere);

Vector3 GetClosestPointOnOBB(const Sphere& sphere, const Vector3& velocity, const AABB& aabb);
Vector3 GetClosestPointOnOBB(const Sphere& sphere, const Vector3& velocity, const OBB& obb);
Vector3 GetClosestPointOnOBB(const Sphere& sphere0, const Vector3& velocity, const Sphere& sphere1);

Vector3 CalculateReflection(const Vector3& incoming, const Vector3& normal);

EulerTransformData ExtractTransform(const Matrix4x4& matrix);

int getDigits(int number, int position);