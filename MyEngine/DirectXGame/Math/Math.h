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

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize();
};

struct QuaternionTransformData {
	Vector3 scale_;
	Quaternion rotate_;
	Vector3 translate_;

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize();
};

static const int kColumnWidth = 60;
static const int kRowHeight = 20;

#pragma region Vectror3

/// <summary>
/// 加算関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Add(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 減算関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

/// <summary>
/// スカラー倍する関数
/// </summary>
/// <param name="scalar"></param>
/// <param name="v"></param>
/// <returns></returns>
Vector3 Multiply(float scalar, const Vector3& v);

/// <summary>
/// ベクトルの内積を求める関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Dot(const Vector3& v1, const Vector3& v2);

/// <summary>
/// ベクトルの長さを計算する関数
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
float Length(const Vector3& v);

/// <summary>
/// 正規化する関数
/// </summary>
/// <param name="v1"></param>
/// <returns></returns>
Vector3 Normalize(const Vector3& v1);

/// <summary>
/// 外積を求める関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 面の表か裏かを判定する処理
/// </summary>
/// <param name="v1"></param>
/// <param name="obj"></param>
/// <returns></returns>
bool IsFront(const Vector3& v1, const Vector3 obj[3]);

/// <summary>
/// ベクトルv1をベクトルv2に射影した結果を返します。
/// </summary>
Vector3 Project(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 指定された線分上の最も近い点を返します。
/// </summary>
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

/// <summary>
/// ベクトルvを行列mで変換し、法線ベクトルとしての結果を返します。
/// </summary>
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

/// <summary>
/// 指定されたベクトルに対する垂直なベクトルを計算します。
/// </summary>
Vector3 Perpendicular(const Vector3& v);

/// <summary>
/// 基準位置(basePosition)から高さ(height)分だけ下に移動した位置を返します。
/// </summary>
Vector3 GetPivotBottomPosition(const Vector3& basePosition, float height);

/// <summary>
/// 2つのベクトル間を補間した結果を返します。
/// </summary>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

/// <summary>
/// 2つのベクトル間を球面線形補間した結果を返します。
/// </summary>
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

#pragma endregion

#pragma region Matrix4x4

/// <summary>
/// 4x4行列の乗算を行います。
/// </summary>
Matrix4x4 operator*(const Matrix4x4& num1, const Matrix4x4& num2);

/// <summary>
/// 4x4行列に他の行列を掛けた結果を代入します。
/// </summary>
Matrix4x4 operator*=(Matrix4x4& num1, const Matrix4x4& num2);

/// <summary>
/// 2つの4x4行列が等しいかどうかを判定します。
/// </summary>
bool operator==(const Matrix4x4& num1, const Matrix4x4& num2);

/// <summary>
/// 4x4行列の加算を行います。
/// </summary>
Matrix4x4 Add(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

/// <summary>
/// 4x4行列の減算を行います。
/// </summary>
Matrix4x4 Subtract(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

/// <summary>
/// 4x4行列の積を計算します。
/// </summary>
Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

/// <summary>
/// 4x4行列の行列式を計算します。
/// </summary>
float Det(const Matrix4x4& matrix);

/// <summary>
/// 4x4行列の逆行列を計算します。
/// </summary>
Matrix4x4 Inverse(const Matrix4x4& matrix);

/// <summary>
/// 4x4行列の転置行列を計算します。
/// </summary>
Matrix4x4 Transpose(const Matrix4x4& matrix);

/// <summary>
/// 4x4の単位行列を作成します。
/// </summary>
Matrix4x4 MakeIdentity4x4();

/// <summary>
/// X軸回転行列を作成します。
/// </summary>
Matrix4x4 MakeRotateXMatrix(float radian);

/// <summary>
/// Y軸回転行列を作成します。
/// </summary>
Matrix4x4 MakeRotateYMatrix(float radian);

/// <summary>
/// Z軸回転行列を作成します。
/// </summary>
Matrix4x4 MakeRotateZMatrix(float radian);

/// <summary>
/// X, Y, Zの回転行列を組み合わせた回転行列を作成します。
/// </summary>
Matrix4x4 MakeRotateXYZMatrix(const Matrix4x4& matrixX, const Matrix4x4& matrixY, const Matrix4x4& matrixZ);

/// <summary>
/// 指定されたベクトルに基づく回転行列を作成します。
/// </summary>
Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

/// <summary>
/// XZY順のオイラー角を回転行列から抽出します。
/// </summary>
Vector3 ExtractEulerAnglesFromMatrixXZY(const Matrix3x3& rotationMatrix);

/// <summary>
/// XZY順のオイラー角を4x4行列から抽出します。
/// </summary>
Vector3 ExtractEulerAnglesFromMatrixXZY(const Matrix4x4& matrix);

/// <summary>
/// 平行移動行列を作成します。
/// </summary>
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/// <summary>
/// スケール行列を作成します。
/// </summary>
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

/// <summary>
/// スケール、回転、平行移動を組み合わせたアフィン行列を作成します。
/// </summary>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

/// <summary>
/// スケール、回転、平行移動を組み合わせたアフィン行列を作成します。
/// </summary>
Matrix4x4 MakeAffineMatrix(const EulerTransformData& transformData);

/// <summary>
/// スケール、回転、平行移動を組み合わせたアフィン行列を作成します。
/// </summary>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

/// <summary>
/// スケール、回転、平行移動を組み合わせたアフィン行列を作成します。
/// </summary>
Matrix4x4 MakeAffineMatrix(const QuaternionTransformData& transformData);

/// <summary>
/// ベクトルをマトリックスの向きに回転させる関数
/// </summary>
/// <param name="vector"></param>
/// <param name="matrix"></param>
/// <returns></returns>
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

/// <summary>
/// 視野角を指定して透視投影行列を作成します。
/// </summary>
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

/// <summary>
/// 正射影行列を作成します。
/// </summary>
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

/// <summary>
/// ビューポート行列を作成します。
/// </summary>
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

/// <summary>
/// 指定した点がAABBに衝突しているかを判定します。
/// </summary>
bool IsCollision(const Vector3& point, const AABB& aabb);

#pragma endregion

#pragma region OBB

/// <summary>
/// OBBと球の衝突を判定します。
/// </summary>
bool IsCollision(const OBB& obb, const Sphere& sphere);

/// <summary>
/// OBBと球の衝突を判定します。
/// </summary>
bool IsCollision(const Sphere& sphere, const OBB& obb);

/// <summary>
/// OBBとOBBの接触している法線を判定する処理
/// </summary>
/// <param name="obb1"></param>
/// <param name="obb2"></param>
/// <param name="axis"></param>
/// <returns></returns>
bool OverlapOnAxis(const OBB& obb1, const OBB& obb2, const Vector3& axis);

/// <summary>
/// OBBとOBBの衝突を判定します。
/// </summary>
bool IsCollision(const OBB& obb1, const OBB& obb2);

/// <summary>
/// OBBとAABBの衝突を判定します。
/// </summary>
bool IsCollision(const OBB& obb, const AABB& aabb);

/// <summary>
/// AABBとOBBの衝突を判定します。
/// </summary>
bool IsCollision(const AABB& aabb, const OBB& obb);

#pragma endregion

#pragma region Sphere

/// <summary>
/// 球と球の衝突を判定します。
/// </summary>
bool IsCollision(const Sphere& s1, const Sphere& s2);

/// <summary>
/// 球とAABBの衝突を判定します。
/// </summary>
bool IsCollision(const Sphere& sphere, const AABB& aabb);

/// <summary>
/// AABBと球の衝突を判定します。
/// </summary>
bool IsCollision(const AABB& aabb, const Sphere& sphere);

#pragma endregion

/// <summary>
/// AABBのmaxとminをclampする処理
/// </summary>
void ControlMinMax(AABB& a);

/// <summary>
/// 数値をminからmaxの間に調整する処理
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
float Clamp(float value, float min, float max);

/// <summary>
/// AABBとAABBの衝突を判定します。
/// </summary>
bool IsCollision(const AABB& a, const AABB& b);

/// <summary>
/// AABBと頂点の衝突を判定します。
/// </summary>
bool IsCollision(const AABB& aabb, const Vector3& point);

/// <summary>
/// AABBとAABBの接触面の法線を求める処理
/// </summary>
Vector3 CalculateNormal(const AABB& a, const AABB& b);

/// <summary>
/// AABBとOBBの接触面の法線を求める処理
/// </summary>
Vector3 CalculateNormal(const AABB& a, const OBB& b);

/// <summary>
/// AABBと球の接触面の法線を求める処理
/// </summary>
Vector3 CalculateNormal(const AABB& a, const Sphere& b);

/// <summary>
/// OBBとOBBの接触面の法線を求める処理
/// </summary>
Vector3 CalculateNormal(const OBB& a, const OBB& b);

/// <summary>
/// OBBとAABBの接触面の法線を求める処理
/// </summary>
Vector3 CalculateNormal(const OBB& a, const AABB& b);

/// <summary>
/// OBBと球の接触面の法線を求める処理
/// </summary>
Vector3 CalculateNormal(const OBB& a, const Sphere& b);

/// <summary>
/// 球と球の接触面の法線を求める処理
/// </summary>
Vector3 CalculateNormal(const Sphere& a, const Sphere& b);

/// <summary>
/// 球とAABBの接触面の法線を求める処理
/// </summary>
Vector3 CalculateNormal(const Sphere& a, const AABB& b);

/// <summary>
/// 球とOBBの接触面の法線を求める処理
/// </summary>
Vector3 CalculateNormal(const Sphere& a, const OBB& b);

/// <summary>
/// 衝突点を考慮したOBB上の最も近い点を計算します。
/// </summary>
Vector3 ClosestPointOnOBB(const OBB& obb, const Vector3& point);

/// <summary>
/// 衝突点を考慮したOBB上の最も近い点を計算します。
/// </summary>
Vector3 GetClosestPointOnOBB(const AABB& aabb0, const Vector3& velocity, const AABB& aabb1);

/// <summary>
/// 衝突点を考慮したOBB上の最も近い点を計算します。
/// </summary>
Vector3 GetClosestPointOnOBB(const AABB& aabb, const Vector3& velocity, const OBB& obb);

/// <summary>
/// 衝突点を考慮したOBB上の最も近い点を計算します。
/// </summary>
Vector3 GetClosestPointOnOBB(const AABB& aabb, const Vector3& velocity, const Sphere& sphere);

/// <summary>
/// 衝突点を考慮したOBB上の最も近い点を計算します。
/// </summary>
Vector3 GetClosestPointOnOBB(const OBB& obb, const Vector3& velocity, const AABB& aabb);

/// <summary>
/// 衝突点を考慮したOBB上の最も近い点を計算します。
/// </summary>
Vector3 GetClosestPointOnOBB(const OBB& obb0, const Vector3& velocity, const OBB& obb1);

/// <summary>
/// 衝突点を考慮したOBB上の最も近い点を計算します。
/// </summary>
Vector3 GetClosestPointOnOBB(const OBB& obb, const Vector3& velocity, const Sphere& sphere);

/// <summary>
/// 衝突点を考慮したOBB上の最も近い点を計算します。
/// </summary>
Vector3 GetClosestPointOnOBB(const Sphere& sphere, const Vector3& velocity, const AABB& aabb);

/// <summary>
/// 衝突点を考慮したOBB上の最も近い点を計算します。
/// </summary>
Vector3 GetClosestPointOnOBB(const Sphere& sphere, const Vector3& velocity, const OBB& obb);

/// <summary>
/// 衝突点を考慮したOBB上の最も近い点を計算します。
/// </summary>
Vector3 GetClosestPointOnOBB(const Sphere& sphere0, const Vector3& velocity, const Sphere& sphere1);

/// <summary>
/// 衝突点を考慮したOBB上の最も近い点を計算します。
/// </summary>
Vector3 CalculateReflection(const Vector3& incoming, const Vector3& normal);

/// <summary>
/// 行列から変換データを抽出します。
/// </summary>
EulerTransformData ExtractTransform(const Matrix4x4& matrix);

/// <summary>
/// 数字の指定した桁数を取得します。
/// </summary>
int getDigits(int number, int position);