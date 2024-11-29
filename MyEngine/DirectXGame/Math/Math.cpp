#include "Math.h"

#pragma region Vector3
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 v3 = {0, 0, 0};

	v3.x = v1.x + v2.x;

	v3.y = v1.y + v2.y;
	
	v3.z = v1.z + v2.z;

	return v3;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 v3 = { 0, 0, 0 };

	v3.x = v1.x - v2.x;

	v3.y = v1.y - v2.y;

	v3.z = v1.z - v2.z;

	return v3;
}

Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 v3 = { 0, 0, 0 };

	v3.x = scalar * v.x;
 
	v3.y = scalar * v.y;
 
	v3.z = scalar * v.z;

	return v3;
}

float Dot(const Vector3& v1, const Vector3& v2) {
	float dot = 0.0f;

	dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return dot;
}

float Length(const Vector3& v) {
	float length = 0;

	length = sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));

	return length;
}

Vector3 Normalize(const Vector3& v1) {
	Vector3 v2 = { 0, 0, 0 };
	float length = Length(v1);

	v2.x = v1.x / length;
	v2.y = v1.y / length;
	v2.z = v1.z / length;

	return v2;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 v3{};
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;

	return v3;
}

bool IsFront(const Vector3& v1, const Vector3 obj[3]) {

	Vector3 vecA = obj[1] - obj[0];
	Vector3 vecB = obj[2] - obj[1];

	Vector3 v2 = Cross(vecA, vecB);

	float dot = Dot(v1, v2);

	if (dot <= 0) {
		return true;
	}
	return false;
}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float scalar = Dot(v1, Normalize(v2));

	return Multiply(scalar, Normalize(v2));
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	Vector3 cp = segment.origin + project;
	return cp;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	};
	return result;
}

Vector3 Perpendicular(const Vector3& v) {
	if (v.x != 0.0f || v.y != 0.0f) {
		return{ -v.y, v.x, 0.0f };
	}
	return{ 0.0f, -v.z, v.y };
}

Vector3 GetPivotBottomPosition(const Vector3& basePosition, float height) {
	return { basePosition.x, basePosition.y - height / 2.0f, basePosition.z };
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 v3 = {};

	v3.x = v1.x + t * (v2.x - v1.x);
	v3.y = v1.y + t * (v2.y - v1.y);
	v3.z = v1.z + t * (v2.z - v1.z);

	return v3;
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 v3 = {};

	if (v1 == v2) {
		v3 = Lerp(v1, v2, t);
		return v3;
	}
	else {
		// 2ベクトル間の角度（鋭角側）
		float angle = std::acos(Dot(v1, v2));

		// sinθ
		float SinTh = std::sin(angle);

		// 補間係数
		float Ps = std::sin(angle * (1 - t));
		float Pe = std::sin(angle * t);

		v3.x = (Ps * v1.x + Pe * v2.x) / SinTh;
		v3.y = (Ps * v1.y + Pe * v2.y) / SinTh;
		v3.z = (Ps * v1.z + Pe * v2.z) / SinTh;

		// 一応正規化して球面線形補間に
		Normalize(v3);

		return v3;
	}
}

#pragma endregion

#pragma region Matrix4x4

Matrix4x4 operator*(const Matrix4x4& num1, const Matrix4x4& num2) {
	return Multiply(num1, num2);
}

Matrix4x4 operator*=(Matrix4x4& num1, const Matrix4x4& num2) {
	num1 = Multiply(num1, num2);
	return num1;
}

bool operator==(const Matrix4x4& num1, const Matrix4x4& num2) {
	bool orTF = true;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (num1.m[y][x] != num2.m[y][x]) {
				orTF = false;
			}
		}
	}
	return orTF;
}

Matrix4x4 Add(const Matrix4x4& matrix1, const Matrix4x4& matrix2) {
	Matrix4x4 matrix = {};
	for (int row = 0; row < 4; row++) {
		for (int colmun = 0; colmun < 4; colmun++) {
			matrix.m[row][colmun] = matrix1.m[row][colmun] + matrix2.m[row][colmun];
		}
	}
	return matrix;
}

Matrix4x4 Subtract(const Matrix4x4& matrix1, const Matrix4x4& matrix2) {
	Matrix4x4 matrix = {};
	for (int row = 0; row < 4; row++) {
		for (int colmun = 0; colmun < 4; colmun++) {
			matrix.m[row][colmun] = matrix1.m[row][colmun] - matrix2.m[row][colmun];
		}
	}
	return matrix;
}

Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2) {
	Matrix4x4 matrix = {};

	matrix.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0] + matrix1.m[0][2] * matrix2.m[2][0] + matrix1.m[0][3] * matrix2.m[3][0];
	matrix.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1] + matrix1.m[0][2] * matrix2.m[2][1] + matrix1.m[0][3] * matrix2.m[3][1];
	matrix.m[0][2] = matrix1.m[0][0] * matrix2.m[0][2] + matrix1.m[0][1] * matrix2.m[1][2] + matrix1.m[0][2] * matrix2.m[2][2] + matrix1.m[0][3] * matrix2.m[3][2];
	matrix.m[0][3] = matrix1.m[0][0] * matrix2.m[0][3] + matrix1.m[0][1] * matrix2.m[1][3] + matrix1.m[0][2] * matrix2.m[2][3] + matrix1.m[0][3] * matrix2.m[3][3];

	matrix.m[1][0] = matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0] + matrix1.m[1][2] * matrix2.m[2][0] + matrix1.m[1][3] * matrix2.m[3][0];
	matrix.m[1][1] = matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1] + matrix1.m[1][2] * matrix2.m[2][1] + matrix1.m[1][3] * matrix2.m[3][1];
	matrix.m[1][2] = matrix1.m[1][0] * matrix2.m[0][2] + matrix1.m[1][1] * matrix2.m[1][2] + matrix1.m[1][2] * matrix2.m[2][2] + matrix1.m[1][3] * matrix2.m[3][2];
	matrix.m[1][3] = matrix1.m[1][0] * matrix2.m[0][3] + matrix1.m[1][1] * matrix2.m[1][3] + matrix1.m[1][2] * matrix2.m[2][3] + matrix1.m[1][3] * matrix2.m[3][3];

	matrix.m[2][0] = matrix1.m[2][0] * matrix2.m[0][0] + matrix1.m[2][1] * matrix2.m[1][0] + matrix1.m[2][2] * matrix2.m[2][0] + matrix1.m[2][3] * matrix2.m[3][0];
	matrix.m[2][1] = matrix1.m[2][0] * matrix2.m[0][1] + matrix1.m[2][1] * matrix2.m[1][1] + matrix1.m[2][2] * matrix2.m[2][1] + matrix1.m[2][3] * matrix2.m[3][1];
	matrix.m[2][2] = matrix1.m[2][0] * matrix2.m[0][2] + matrix1.m[2][1] * matrix2.m[1][2] + matrix1.m[2][2] * matrix2.m[2][2] + matrix1.m[2][3] * matrix2.m[3][2];
	matrix.m[2][3] = matrix1.m[2][0] * matrix2.m[0][3] + matrix1.m[2][1] * matrix2.m[1][3] + matrix1.m[2][2] * matrix2.m[2][3] + matrix1.m[2][3] * matrix2.m[3][3];

	matrix.m[3][0] = matrix1.m[3][0] * matrix2.m[0][0] + matrix1.m[3][1] * matrix2.m[1][0] + matrix1.m[3][2] * matrix2.m[2][0] + matrix1.m[3][3] * matrix2.m[3][0];
	matrix.m[3][1] = matrix1.m[3][0] * matrix2.m[0][1] + matrix1.m[3][1] * matrix2.m[1][1] + matrix1.m[3][2] * matrix2.m[2][1] + matrix1.m[3][3] * matrix2.m[3][1];
	matrix.m[3][2] = matrix1.m[3][0] * matrix2.m[0][2] + matrix1.m[3][1] * matrix2.m[1][2] + matrix1.m[3][2] * matrix2.m[2][2] + matrix1.m[3][3] * matrix2.m[3][2];
	matrix.m[3][3] = matrix1.m[3][0] * matrix2.m[0][3] + matrix1.m[3][1] * matrix2.m[1][3] + matrix1.m[3][2] * matrix2.m[2][3] + matrix1.m[3][3] * matrix2.m[3][3];

	return matrix;
}

float Det(const Matrix4x4& matrix) {
	float det = 0.0f;

	det = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3]
		+ matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1]
		+ matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2]

		- matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1]
		- matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3]
		- matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]

		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3]
		- matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]
		- matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2]

		+ matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]
		+ matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3]
		+ matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]

		+ matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3]
		+ matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1]
		+ matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2]

		- matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1]
		- matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3]
		- matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2]

		- matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0]
		- matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0]
		- matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0]

		+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0]
		+ matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0]
		+ matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0];

	return det;
}

Matrix4x4 Inverse(const Matrix4x4& matrix) {
	Matrix4x4 matrix1 = {};
	float det = Det(matrix);

	matrix1.m[0][0] = (1.0f / det) * (matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3]
		+ matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1]
		+ matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2]
		- matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1]
		- matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3]
		- matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]);
	matrix1.m[0][1] = (1.0f / det) * (-1 * matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3]
		- matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1]
		- matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2]
		+ matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1]
		+ matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3]
		+ matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2]);
	matrix1.m[0][2] = (1.0f / det) * (matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3]
		+ matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1]
		+ matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2]
		- matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1]
		- matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3]
		- matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2]);
	matrix1.m[0][3] = (1.0f / det) * (-1 * matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3]
		- matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1]
		- matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2]
		+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1]
		+ matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3]
		+ matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2]);

	matrix1.m[1][0] = (1.0f / det) * (-1 * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3]
		- matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0]
		- matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2]
		+ matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0]
		+ matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3]
		+ matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]);
	matrix1.m[1][1] = (1.0f / det) * (matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3]
		+ matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0]
		+ matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2]
		- matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0]
		- matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3]
		- matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2]);
	matrix1.m[1][2] = (1.0f / det) * (-1 * matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3]
		- matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0]
		- matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2]
		+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0]
		+ matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3]
		+ matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2]);
	matrix1.m[1][3] = (1.0f / det) * (matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3]
		+ matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0]
		+ matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2]
		- matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0]
		- matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3]
		- matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2]);

	matrix1.m[2][0] = (1.0f / det) * (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3]
		+ matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0]
		+ matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1]
		- matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0]
		- matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3]
		- matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]);
	matrix1.m[2][1] = (1.0f / det) * (-1 * matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3]
		- matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0]
		- matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1]
		+ matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0]
		+ matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3]
		+ matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1]);
	matrix1.m[2][2] = (1.0f / det) * (matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3]
		+ matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0]
		+ matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1]
		- matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0]
		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3]
		- matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1]);
	matrix1.m[2][3] = (1.0f / det) * (-1 * matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3]
		- matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0]
		- matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1]
		+ matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0]
		+ matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3]
		+ matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1]);

	matrix1.m[3][0] = (1.0f / det) * (-1 * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2]
		- matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0]
		- matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1]
		+ matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0]
		+ matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2]
		+ matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]);
	matrix1.m[3][1] = (1.0f / det) * (matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2]
		+ matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0]
		+ matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1]
		- matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0]
		- matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2]
		- matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1]);
	matrix1.m[3][2] = (1.0f / det) * (-1 * matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2]
		- matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0]
		- matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1]
		+ matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0]
		+ matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2]
		+ matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1]);
	matrix1.m[3][3] = (1.0f / det) * (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2]
		+ matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0]
		+ matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1]
		- matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0]
		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2]
		- matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]);

	return matrix1;
}

Matrix4x4 Transpose(const Matrix4x4& matrix) {
	Matrix4x4 matrix1 = {};

	matrix1.m[0][0] = matrix.m[0][0];
	matrix1.m[0][1] = matrix.m[1][0];
	matrix1.m[0][2] = matrix.m[2][0];
	matrix1.m[0][3] = matrix.m[3][0];

	matrix1.m[1][0] = matrix.m[0][1];
	matrix1.m[1][1] = matrix.m[1][1];
	matrix1.m[1][2] = matrix.m[2][1];
	matrix1.m[1][3] = matrix.m[3][1];

	matrix1.m[2][0] = matrix.m[0][2];
	matrix1.m[2][1] = matrix.m[1][2];
	matrix1.m[2][2] = matrix.m[2][2];
	matrix1.m[2][3] = matrix.m[3][2];

	matrix1.m[3][0] = matrix.m[0][3];
	matrix1.m[3][1] = matrix.m[1][3];
	matrix1.m[3][2] = matrix.m[2][3];
	matrix1.m[3][3] = matrix.m[3][3];

	return matrix1;
}

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 matrix = {};

	matrix.m[0][0] = 1.0f;
	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;
	matrix.m[0][3] = 0.0f;

	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[1][2] = 0.0f;
	matrix.m[1][3] = 0.0f;

	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[2][3] = 0.0f;

	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][2] = 0.0f;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 matrix = MakeIdentity4x4();

	matrix.m[1][1] = std::cos(radian);
	matrix.m[1][2] = std::sin(radian);

	matrix.m[2][1] = std::sin(radian) * -1;
	matrix.m[2][2] = std::cos(radian);

	return matrix;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 matrix = MakeIdentity4x4();

	matrix.m[0][0] = std::cos(radian);
	matrix.m[0][2] = std::sin(radian) * -1;

	matrix.m[2][0] = std::sin(radian);
	matrix.m[2][2] = std::cos(radian);

	return matrix;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 matrix = MakeIdentity4x4();

	matrix.m[0][0] = std::cos(radian);
	matrix.m[0][1] = std::sin(radian);

	matrix.m[1][0] = std::sin(radian) * -1;
	matrix.m[1][1] = std::cos(radian);

	return matrix;
}

Matrix4x4 MakeRotateXYZMatrix(const Matrix4x4& matrixX, const Matrix4x4& matrixY, const Matrix4x4& matrixZ) {
	return Multiply(matrixX, Multiply(matrixY, matrixZ));
}

Matrix4x4 MakeRotateMatrix(const Vector3& rotate) {
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	return MakeRotateXYZMatrix(rotateXMatrix, rotateZMatrix, rotateYMatrix);
}

Vector3 ExtractEulerAnglesFromMatrixXZY(const Matrix3x3& rotationMatrix) {
	Vector3 eulerAngles = { 0.0f, 0.0f, 0.0f };

	// 要素を読み取る
	float r00 = rotationMatrix.m[0][0];
	float r01 = rotationMatrix.m[0][1];
	float r02 = rotationMatrix.m[0][2];
	float r21 = rotationMatrix.m[2][1];
	float r22 = rotationMatrix.m[2][2];

	// Z回転（roll）
	eulerAngles.z = atan2(-r01, r00);

	// Y回転（yaw）
	eulerAngles.y = asin(r02);

	// X回転（pitch）
	eulerAngles.x = atan2(r21, r22);

	return eulerAngles;
}

Vector3 ExtractEulerAnglesFromMatrixXZY(const Matrix4x4& matrix) {
	Vector3 eulerAngles = { 0.0f, 0.0f, 0.0f };

	// 3x3回転成分を読み取る
	float m00 = matrix.m[0][0];
	float m01 = matrix.m[0][1];
	float m02 = matrix.m[0][2];
	float m21 = matrix.m[2][1];
	float m22 = matrix.m[2][2];

	// Z回転（roll）
	eulerAngles.z = atan2(-m01, m00);

	// Y回転（yaw）
	eulerAngles.y = asin(m02);

	// X回転（pitch）
	eulerAngles.x = atan2(m21, m22);

	return eulerAngles;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 matrix = MakeIdentity4x4();
	matrix.m[3][0] = translate.x;
	matrix.m[3][1] = translate.y;
	matrix.m[3][2] = translate.z;

	return matrix;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 matrix = {};

	matrix.m[0][0] = scale.x;
	matrix.m[1][1] = scale.y;
	matrix.m[2][2] = scale.z;
	matrix.m[3][3] = 1;

	return matrix;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result = {};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXYZMatrix = MakeRotateMatrix(rotate);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	return Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
}

Matrix4x4 MakeAffineMatrix(const EulerTransformData& transformData) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(transformData.scale_);
	Matrix4x4 rotateXYZMatrix = MakeRotateMatrix(transformData.rotate_);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(transformData.translate_);

	return Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXYZMatrix = MakeRotateMatrix(rotate);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	return Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
}

Matrix4x4 MakeAffineMatrix(const QuaternionTransformData& transformData) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(transformData.scale_);
	Matrix4x4 rotateXYZMatrix = MakeRotateMatrix(transformData.rotate_);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(transformData.translate_);

	return Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 matrix = { 0 };

	matrix.m[0][0] = (1.0f / aspectRatio) * (1.0f / tanf(fovY / 2.0f));
	matrix.m[1][1] = (1.0f / tanf(fovY / 2.0f));
	matrix.m[2][2] = farClip / (farClip - nearClip);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][2] = (-1.0f * nearClip * farClip) / (farClip - nearClip);


	return matrix;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 matrix = MakeIdentity4x4();

	matrix.m[0][0] = 2.0f / (right - left);
	matrix.m[1][1] = 2.0f / (top - bottom);
	matrix.m[2][2] = 1.0f / (farClip - nearClip);

	matrix.m[3][0] = (left + right) / (left - right);
	matrix.m[3][1] = (top + bottom) / (bottom - top);
	matrix.m[3][2] = (nearClip) / (nearClip - farClip);

	return matrix;
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 matrix = MakeIdentity4x4();

	matrix.m[0][0] = width / 2.0f;
	matrix.m[1][1] = -1 * (height / 2.0f);
	matrix.m[2][2] = maxDepth - minDepth;

	matrix.m[3][0] = left + width / 2.0f;
	matrix.m[3][1] = top + height / 2.0f;
	matrix.m[3][2] = minDepth;

	return matrix;
}
#pragma endregion

#pragma region OBB

bool IsCollision(const OBB& obb, const Sphere& sphere) {
	Matrix4x4 worldMatrix = MakeIdentity4x4();
	worldMatrix.m[0][0] = obb.orientations[0].x;
	worldMatrix.m[0][1] = obb.orientations[0].y;
	worldMatrix.m[0][2] = obb.orientations[0].z;

	worldMatrix.m[1][0] = obb.orientations[1].x;
	worldMatrix.m[1][1] = obb.orientations[1].y;
	worldMatrix.m[1][2] = obb.orientations[1].z;

	worldMatrix.m[2][0] = obb.orientations[2].x;
	worldMatrix.m[2][1] = obb.orientations[2].y;
	worldMatrix.m[2][2] = obb.orientations[2].z;

	worldMatrix.m[3][0] = obb.center.x;
	worldMatrix.m[3][1] = obb.center.y;
	worldMatrix.m[3][2] = obb.center.z;

	Matrix4x4 worldMatrixInverse = Inverse(worldMatrix);
	Vector3 centerInOBBLocalSpace = Transform(sphere.center, worldMatrixInverse);
	AABB aabbLocal{ .min = {-obb.size.x / 2.0f, -obb.size.y / 2.0f, -obb.size.z / 2.0f}, .max{obb.size.x / 2.0f, obb.size.y / 2.0f, obb.size.z / 2.0f} };
	Sphere sphereOBBLocal{ .center = centerInOBBLocalSpace, .radius = sphere.radius };
	if (IsCollision(sphereOBBLocal, aabbLocal)) {
		return true;
	}
	return false;
}

bool IsCollision(const Sphere& sphere, const OBB& obb) {
	Matrix4x4 worldMatrix = MakeIdentity4x4();
	worldMatrix.m[0][0] = obb.orientations[0].x;
	worldMatrix.m[0][1] = obb.orientations[0].y;
	worldMatrix.m[0][2] = obb.orientations[0].z;

	worldMatrix.m[1][0] = obb.orientations[1].x;
	worldMatrix.m[1][1] = obb.orientations[1].y;
	worldMatrix.m[1][2] = obb.orientations[1].z;

	worldMatrix.m[2][0] = obb.orientations[2].x;
	worldMatrix.m[2][1] = obb.orientations[2].y;
	worldMatrix.m[2][2] = obb.orientations[2].z;

	worldMatrix.m[3][0] = obb.center.x;
	worldMatrix.m[3][1] = obb.center.y;
	worldMatrix.m[3][2] = obb.center.z;

	Matrix4x4 worldMatrixInverse = Inverse(worldMatrix);
	Vector3 centerInOBBLocalSpace = Transform(sphere.center, worldMatrixInverse);
	AABB aabbLocal{ .min = {-obb.size.x / 2.0f, -obb.size.y / 2.0f, -obb.size.z / 2.0f}, .max{obb.size.x / 2.0f, obb.size.y / 2.0f, obb.size.z / 2.0f} };
	Sphere sphereOBBLocal{ .center = centerInOBBLocalSpace, .radius = sphere.radius };
	if (IsCollision(sphereOBBLocal, aabbLocal)) {
		return true;
	}
	return false;
}

bool OverlapOnAxis(const OBB& obb1, const OBB& obb2, const Vector3& axis) {
	// Project both OBBs onto the axis and check for overlap
	float obb1Projection =
		fabs(Dot(obb1.orientations[0], axis)) * (obb1.size.x / 2.0f) +
		fabs(Dot(obb1.orientations[1], axis)) * (obb1.size.y / 2.0f) +
		fabs(Dot(obb1.orientations[2], axis)) * (obb1.size.z / 2.0f);

	float obb2Projection =
		fabs(Dot(obb2.orientations[0], axis)) * (obb2.size.x / 2.0f) +
		fabs(Dot(obb2.orientations[1], axis)) * (obb2.size.y / 2.0f) +
		fabs(Dot(obb2.orientations[2], axis)) * (obb2.size.z / 2.0f);

	float distance = fabs(Dot((obb2.center - obb1.center), axis));

	return distance <= obb1Projection + obb2Projection;
}

bool IsCollision(const OBB& obb1, const OBB& obb2) {
	// Test the 6 axes of the two OBBs
	for (int i = 0; i < 3; ++i) {
		if (!OverlapOnAxis(obb1, obb2, obb1.orientations[i])) {
			return false;
		}
		if (!OverlapOnAxis(obb1, obb2, obb2.orientations[i])) {
			return false;
		}
	}

	// Test the 9 cross products of the axes
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			Vector3 axis = Cross(obb1.orientations[i], obb2.orientations[j]);
			if (axis.x != 0 || axis.y != 0 || axis.z != 0) { // Check for zero vector
				axis = Normalize(axis);
				if (!OverlapOnAxis(obb1, obb2, axis)) {
					return false;
				}
			}
		}
	}

	return true; // No separating axis found, OBBs must be intersecting
}

bool IsCollision(const OBB& obb1, const AABB& aabb) {

	Vector3 aabbCenter = { (aabb.min.x + aabb.max.x) / 2.0f, (aabb.min.y + aabb.max.y) / 2.0f, (aabb.min.z + aabb.max.z) / 2.0f };

	OBB obb2;
	obb2.center = aabbCenter;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0, 0, 0 });
	obb2.orientations[0].x = rotateMatrix.m[0][0];
	obb2.orientations[0].y = rotateMatrix.m[0][1];
	obb2.orientations[0].z = rotateMatrix.m[0][2];

	obb2.orientations[1].x = rotateMatrix.m[1][0];
	obb2.orientations[1].y = rotateMatrix.m[1][1];
	obb2.orientations[1].z = rotateMatrix.m[1][2];

	obb2.orientations[2].x = rotateMatrix.m[2][0];
	obb2.orientations[2].y = rotateMatrix.m[2][1];
	obb2.orientations[2].z = rotateMatrix.m[2][2];

	obb2.size = { aabb.max.x - aabb.min.x, aabb.max.y - aabb.min.y , aabb.max.z - aabb.min.z };

	return IsCollision(obb1, obb2);
}

bool IsCollision(const AABB& aabb, const OBB& obb1) {

	Vector3 aabbCenter = { (aabb.min.x + aabb.max.x) / 2.0f, (aabb.min.y + aabb.max.y) / 2.0f, (aabb.min.z + aabb.max.z) / 2.0f };

	OBB obb2;
	obb2.center = aabbCenter;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0, 0, 0 });
	obb2.orientations[0].x = rotateMatrix.m[0][0];
	obb2.orientations[0].y = rotateMatrix.m[0][1];
	obb2.orientations[0].z = rotateMatrix.m[0][2];

	obb2.orientations[1].x = rotateMatrix.m[1][0];
	obb2.orientations[1].y = rotateMatrix.m[1][1];
	obb2.orientations[1].z = rotateMatrix.m[1][2];

	obb2.orientations[2].x = rotateMatrix.m[2][0];
	obb2.orientations[2].y = rotateMatrix.m[2][1];
	obb2.orientations[2].z = rotateMatrix.m[2][2];

	obb2.size = { aabb.max.x - aabb.min.x, aabb.max.y - aabb.min.y , aabb.max.z - aabb.min.z };

	return IsCollision(obb1, obb2);
}

#pragma endregion

#pragma region Sphere

bool IsCollision(const Sphere& s1, const Sphere& s2) {
	auto distance = Length(s2.center - s1.center);

	if (distance <= s1.radius + s2.radius) {
		return true;
	}

	return false;
}

bool IsCollision(const Sphere& sphere, const AABB& aabb) {
	Vector3 closestPoint = {
		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z),
	};

	float distance = Length(closestPoint - sphere.center);

	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}

bool IsCollision(const Vector3& point, const AABB& aabb) {

	if ((point.x >= aabb.min.x && point.x <= aabb.max.x) &&
		(point.y >= aabb.min.y && point.y <= aabb.max.y) &&
		(point.z >= aabb.min.z && point.z <= aabb.max.z)) {

		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Sphere& sphere) {
	Vector3 closestPoint = {
		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z),
	};

	float distance = Length(closestPoint - sphere.center);

	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}

#pragma endregion

void ControlMinMax(AABB& a) {
	a.min.x = (std::min)(a.min.x, a.max.x);
	a.max.x = (std::max)(a.min.x, a.max.x);

	a.min.y = (std::min)(a.min.y, a.max.y);
	a.max.y = (std::max)(a.min.y, a.max.y);

	a.min.z = (std::min)(a.min.z, a.max.z);
	a.max.z = (std::max)(a.min.z, a.max.z);
}

float Clamp(float value, float min, float max) {
	return std::max(min, std::min(value, max));
}

Vector3 ClosestPointOnOBB(const OBB& obb, const Vector3& point) {
	// OBB の中心からのベクトル
	Vector3 d = point - obb.center;
	Vector3 closest = obb.center;

	// 各軸について計算
	for (int i = 0; i < 3; ++i) {
		// 現在の軸
		Vector3 axis = obb.orientations[i];

		// 軸の正規化を確認（必要に応じて）
		axis = Normalize(axis);

		// 点を現在の軸に投影
		float distance = Dot(d, axis);

		// OBB サイズの半分を計算
		float halfSize = 0.0f;
		if (i == 0) {
			halfSize = obb.size.x / 2.0f;
		}
		else if (i == 1) {
			halfSize = obb.size.y / 2.0f;
		}
		else if (i == 2) {
			halfSize = obb.size.z / 2.0f;
		}

		// 距離を OBB の範囲内にクランプ
		distance = Clamp(distance, -halfSize, halfSize);

		// クローズポイントを更新
		closest += axis * distance;
	}

	return closest;
}

bool IsCollision(const AABB& a, const AABB& b) {
	if (a.min.x <= b.max.x && a.max.x >= b.min.x &&
		a.min.y <= b.max.y && a.max.y >= b.min.y &&
		a.min.z <= b.max.z && a.max.z >= b.min.z) {
		return true;
	}
	return false;
}	

bool IsCollision(const AABB& aabb, const Vector3& point) {
	if (aabb.min.x <= point.x && aabb.max.x >= point.x &&
		aabb.min.y <= point.y && aabb.max.y >= point.y &&
		aabb.min.z <= point.z && aabb.max.z >= point.z) {
		return true;
	}
	return false;
}

Vector3 CalculateNormal(const AABB& a, const AABB& b) {

	if (!IsCollision(a, b)) {
		return Vector3{ 0, 0, 0 };
	}

	Vector3 normal = { 0.0f, 0.0f, 0.0f };

	Vector3 aCenter = { (a.min.x + a.max.x) / 2.0f, (a.min.y + a.max.y) / 2.0f, (a.min.z + a.max.z) / 2.0f };
	Vector3 bCenter = { (b.min.x + b.max.x) / 2.0f, (b.min.y + b.max.y) / 2.0f, (b.min.z + b.max.z) / 2.0f };
	Vector3 aSize = { a.max.x - a.min.x, a.max.y - a.min.y , a.max.z - a.min.z };
	Vector3 bSize = { b.max.x - b.min.x, b.max.y - b.min.y , b.max.z - b.min.z };
	
	Vector3 delta = bCenter - aCenter;
	Vector3 halfSize1 = aSize * 0.5f;
	Vector3 halfSize2 = bSize * 0.5f;

	float overlapX = halfSize1.x + halfSize2.x - abs(delta.x);
	float overlapY = halfSize1.y + halfSize2.y - abs(delta.y);
	float overlapZ = halfSize1.z + halfSize2.z - abs(delta.z);

	if (overlapX < overlapY && overlapX < overlapZ) {
		if (delta.x > 0) {
			normal = { 1.0f, 0.0f, 0.0f };
		}
		else {
			normal = { -1.0f, 0.0f, 0.0f };
		}
	}
	else if (overlapY < overlapX && overlapY < overlapZ) {
		if (delta.y > 0) {
			normal = { 0.0f, 1.0f, 0.0f };
		}
		else {
			normal = { 0.0f, -1.0f, 0.0f };
		}
	}
	else {
		if (delta.z > 0) {
			normal = { 0.0f, 0.0f, 1.0f };
		}
		else {
			normal = { 0.0f, 0.0f, -1.0f };
		}
	}

	normal = Normalize(normal);
	return normal;
}

Vector3 CalculateNormal(const AABB& a, const OBB& b) {
	Vector3 normal = { 0.0f, 0.0f, 0.0f };

	Vector3 aabbCenter = { (a.min.x + a.max.x) / 2.0f, (a.min.y + a.max.y) / 2.0f, (a.min.z + a.max.z) / 2.0f };

	OBB obb;
	obb.center = aabbCenter;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0, 0, 0 });
	obb.orientations[0].x = rotateMatrix.m[0][0];
	obb.orientations[0].y = rotateMatrix.m[0][1];
	obb.orientations[0].z = rotateMatrix.m[0][2];

	obb.orientations[1].x = rotateMatrix.m[1][0];
	obb.orientations[1].y = rotateMatrix.m[1][1];
	obb.orientations[1].z = rotateMatrix.m[1][2];

	obb.orientations[2].x = rotateMatrix.m[2][0];
	obb.orientations[2].y = rotateMatrix.m[2][1];
	obb.orientations[2].z = rotateMatrix.m[2][2];

	obb.size = { a.max.x - a.min.x, a.max.y - a.min.y , a.max.z - a.min.z };

	normal = CalculateNormal(obb, b);

	return normal;
}

Vector3 CalculateNormal(const AABB& a, const Sphere& b) {
	Vector3 aabbCenter = { (a.min.x + a.max.x) / 2.0f, (a.min.y + a.max.y) / 2.0f, (a.min.z + a.max.z) / 2.0f };

	OBB obb;
	obb.center = aabbCenter;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0, 0, 0 });
	obb.orientations[0].x = rotateMatrix.m[0][0];
	obb.orientations[0].y = rotateMatrix.m[0][1];
	obb.orientations[0].z = rotateMatrix.m[0][2];

	obb.orientations[1].x = rotateMatrix.m[1][0];
	obb.orientations[1].y = rotateMatrix.m[1][1];
	obb.orientations[1].z = rotateMatrix.m[1][2];

	obb.orientations[2].x = rotateMatrix.m[2][0];
	obb.orientations[2].y = rotateMatrix.m[2][1];
	obb.orientations[2].z = rotateMatrix.m[2][2];

	obb.size = { a.max.x - a.min.x, a.max.y - a.min.y , a.max.z - a.min.z };

	if (IsCollision(b, a)) {
		Vector3 closestPoint = ClosestPointOnOBB(obb, b.center);
		Vector3 normal = b.center - closestPoint;
		return Normalize(normal);
	}
	return Vector3(0, 0, 0); // No collision, return zero vector
}

Vector3 CalculateNormal([[maybe_unused]] const OBB& a, [[maybe_unused]] const OBB& b) {
	Vector3 normal = { 0.0f, 0.0f, 0.0f };

	return normal;
}

Vector3 CalculateNormal(const OBB& a, const AABB& b) {
	Vector3 normal = { 0.0f, 0.0f, 0.0f };

	Vector3 aabbCenter = { (b.min.x + b.max.x) / 2.0f, (b.min.y + b.max.y) / 2.0f, (b.min.z + b.max.z) / 2.0f };

	OBB obb;
	obb.center = aabbCenter;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0, 0, 0 });
	obb.orientations[0].x = rotateMatrix.m[0][0];
	obb.orientations[0].y = rotateMatrix.m[0][1];
	obb.orientations[0].z = rotateMatrix.m[0][2];

	obb.orientations[1].x = rotateMatrix.m[1][0];
	obb.orientations[1].y = rotateMatrix.m[1][1];
	obb.orientations[1].z = rotateMatrix.m[1][2];

	obb.orientations[2].x = rotateMatrix.m[2][0];
	obb.orientations[2].y = rotateMatrix.m[2][1];
	obb.orientations[2].z = rotateMatrix.m[2][2];

	obb.size = { b.max.x - b.min.x, b.max.y - b.min.y , b.max.z - b.min.z };

	normal = CalculateNormal(a, obb);

	return normal;
}

Vector3 CalculateNormal(const OBB& a, const Sphere& b) {
	if (IsCollision(a, b)) {
		Vector3 closestPoint = ClosestPointOnOBB(a, b.center);
		Vector3 normal = b.center - closestPoint;
		return Normalize(normal);
	}
	return Vector3(0, 0, 0); // No collision, return zero vector
}

Vector3 CalculateNormal(const Sphere& a, const Sphere& b) {
	if (IsCollision(a, b)) {
		Vector3 normal = b.center - a.center;
		return Normalize(normal);
	}
	return Vector3(0, 0, 0);
}

Vector3 CalculateNormal(const Sphere& a, const AABB& b) {
	Vector3 aabbCenter = { (b.min.x + b.max.x) / 2.0f, (b.min.y + b.max.y) / 2.0f, (b.min.z + b.max.z) / 2.0f };

	OBB obb;
	obb.center = aabbCenter;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0, 0, 0 });
	obb.orientations[0].x = rotateMatrix.m[0][0];
	obb.orientations[0].y = rotateMatrix.m[0][1];
	obb.orientations[0].z = rotateMatrix.m[0][2];

	obb.orientations[1].x = rotateMatrix.m[1][0];
	obb.orientations[1].y = rotateMatrix.m[1][1];
	obb.orientations[1].z = rotateMatrix.m[1][2];

	obb.orientations[2].x = rotateMatrix.m[2][0];
	obb.orientations[2].y = rotateMatrix.m[2][1];
	obb.orientations[2].z = rotateMatrix.m[2][2];

	obb.size = { b.max.x - b.min.x, b.max.y - b.min.y , b.max.z - b.min.z };

	if (IsCollision(b, a)) {
		Vector3 normal = CalculateNormal(a, obb);
		return Normalize(normal);
	}
	return Vector3(0, 0, 0); // No collision, return zero vector
}

Vector3 CalculateNormal(const Sphere& a, const OBB& b) {
	if (IsCollision(b, a)) {
		// ワールド行列とその逆行列を計算
		Matrix4x4 worldMatrix = MakeIdentity4x4();
		worldMatrix.m[0][0] = b.orientations[0].x;
		worldMatrix.m[0][1] = b.orientations[0].y;
		worldMatrix.m[0][2] = b.orientations[0].z;

		worldMatrix.m[1][0] = b.orientations[1].x;
		worldMatrix.m[1][1] = b.orientations[1].y;
		worldMatrix.m[1][2] = b.orientations[1].z;

		worldMatrix.m[2][0] = b.orientations[2].x;
		worldMatrix.m[2][1] = b.orientations[2].y;
		worldMatrix.m[2][2] = b.orientations[2].z;

		worldMatrix.m[3][0] = b.center.x;
		worldMatrix.m[3][1] = b.center.y;
		worldMatrix.m[3][2] = b.center.z;

		Matrix4x4 rotateMatrix = MakeIdentity4x4();
		rotateMatrix.m[0][0] = b.orientations[0].x;
		rotateMatrix.m[0][1] = b.orientations[0].y;
		rotateMatrix.m[0][2] = b.orientations[0].z;

		rotateMatrix.m[1][0] = b.orientations[1].x;
		rotateMatrix.m[1][1] = b.orientations[1].y;
		rotateMatrix.m[1][2] = b.orientations[1].z;

		rotateMatrix.m[2][0] = b.orientations[2].x;
		rotateMatrix.m[2][1] = b.orientations[2].y;
		rotateMatrix.m[2][2] = b.orientations[2].z;

		Matrix4x4 worldMatrixInverse = Inverse(worldMatrix);

		// 球の中心をOBBのローカル空間に変換
		Vector3 centerInOBBLocalSpace = Transform(a.center, worldMatrixInverse);

		// ローカル空間のAABBを構築
		AABB aabbLocal{
			.min = {-b.size.x / 2.0f, -b.size.y / 2.0f, -b.size.z / 2.0f},
			.max = { b.size.x / 2.0f,  b.size.y / 2.0f,  b.size.z / 2.0f}
		};

		// AABBの最近傍点を計算
		Vector3 closestPointLocal = {
			std::max(aabbLocal.min.x, std::min(centerInOBBLocalSpace.x, aabbLocal.max.x)),
			std::max(aabbLocal.min.y, std::min(centerInOBBLocalSpace.y, aabbLocal.max.y)),
			std::max(aabbLocal.min.z, std::min(centerInOBBLocalSpace.z, aabbLocal.max.z))
		};

		// 球の中心がOBBの外側か内側かを確認
		bool isInside = (centerInOBBLocalSpace.x >= aabbLocal.min.x && centerInOBBLocalSpace.x <= aabbLocal.max.x &&
			centerInOBBLocalSpace.y >= aabbLocal.min.y && centerInOBBLocalSpace.y <= aabbLocal.max.y &&
			centerInOBBLocalSpace.z >= aabbLocal.min.z && centerInOBBLocalSpace.z <= aabbLocal.max.z);

		// ワールド空間に変換
		Vector3 closestPointWorld = Transform(closestPointLocal, worldMatrix);

		// 法線を計算
		Vector3 normal = { 0.0f, 0.0f, 0.0f };
		if (isInside) {
			// 球がOBB内部の場合、法線を逆向きに
			normal = Normalize(closestPointLocal - centerInOBBLocalSpace);
		}
		else {
			// 球がOBB外部の場合
			normal = Normalize(centerInOBBLocalSpace - closestPointLocal);
		}

		if (fabs(normal.x) > fabs(normal.y) && fabs(normal.x) > fabs(normal.z)) {
			normal = { normal.x > 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f };
		}
		else if (fabs(normal.y) > fabs(normal.x) && fabs(normal.y) > fabs(normal.z)) {
			normal = { 0.0f, normal.y > 0.0f ? 1.0f : -1.0f, 0.0f };
		}
		else {
			normal = { 0.0f, 0.0f, normal.z > 0.0f ? 1.0f : -1.0f };
		}

		normal = Transform(normal, rotateMatrix);

		return normal; // 法線のみ返す場合
	}
	return Vector3(0, 0, 0); // 衝突なし
}

Vector3 GetClosestPointOnOBB([[maybe_unused]] const AABB& aabb0, [[maybe_unused]] const Vector3& velocity, [[maybe_unused]] const AABB& aabb1) {
	Vector3 closest_point = { 0, 0, 0 };

	return closest_point;
}
Vector3 GetClosestPointOnOBB([[maybe_unused]] const AABB& aabb, [[maybe_unused]] const Vector3& velocity, [[maybe_unused]] const OBB& obb) {
	Vector3 closest_point = { 0, 0, 0 };

	return closest_point;
}
Vector3 GetClosestPointOnOBB([[maybe_unused]] const AABB& aabb, [[maybe_unused]] const Vector3& velocity, [[maybe_unused]] const Sphere& sphere) {
	Vector3 closest_point = { 0, 0, 0 };

	return closest_point;
}

Vector3 GetClosestPointOnOBB([[maybe_unused]] const OBB& obb, [[maybe_unused]] const Vector3& velocity, [[maybe_unused]] const AABB& aabb) {
	Vector3 closest_point = { 0, 0, 0 };

	return closest_point;
}

Vector3 GetClosestPointOnOBB([[maybe_unused]] const OBB& obb0, [[maybe_unused]] const Vector3& velocity, [[maybe_unused]] const OBB& obb1) {
	Vector3 closest_point = { 0, 0, 0 };

	return closest_point;
}

Vector3 GetClosestPointOnOBB([[maybe_unused]] const OBB& obb, [[maybe_unused]] const Vector3& velocity, [[maybe_unused]] const Sphere& sphere) {
	Vector3 closest_point = { 0, 0, 0 };

	return closest_point;
}

Vector3 GetClosestPointOnOBB(const Sphere& sphere, const Vector3& velocity, const AABB& aabb) {
	Vector3 closest_point = { 0, 0, 0 };
	Vector3 aabbCenter = { (aabb.min.x + aabb.max.x) / 2.0f, (aabb.min.y + aabb.max.y) / 2.0f, (aabb.min.z + aabb.max.z) / 2.0f };

	if (IsCollision(sphere, aabb)) {

		// スフィアの中心と AABB の最近傍点を計算
		Vector3 nearestPointInAABB = {
			std::max(aabb.min.x, std::min(sphere.center.x, aabb.max.x)),
			std::max(aabb.min.y, std::min(sphere.center.y, aabb.max.y)),
			std::max(aabb.min.z, std::min(sphere.center.z, aabb.max.z))
		};

		// 最近傍点とスフィアの中心の距離を計算
		Vector3 displacement = sphere.center - nearestPointInAABB;
		float distanceSquared = Dot(displacement, displacement);

		// 衝突している場合
		if (distanceSquared <= sphere.radius * sphere.radius) {
			// 接触点をスフィアの表面上に計算
			float distance = sqrt(distanceSquared);
			Vector3 contactPointLocal = nearestPointInAABB;
			if (distance > 0.0f) { // スフィアの中心が AABB 内部にない場合
				contactPointLocal += (displacement / distance) * sphere.radius;
			}

			return closest_point;
		}
	}

	// 衝突していない場合
	return Vector3(0.0f, 0.0f, 0.0f);
}

Vector3 GetClosestPointOnOBB(const Sphere& sphere, const Vector3& velocity, const OBB& obb) {
	Vector3 closest_point = { 0, 0, 0 };

	if (IsCollision(sphere, obb)) {
		Sphere tmpSphere = sphere;
		tmpSphere.center -= velocity;
		while (true) {
			tmpSphere.center += velocity * 0.005f;
			if (IsCollision(tmpSphere, obb)) {
				Vector3 normal = { 0.0f,0.0f,0.0f };
				normal = CalculateNormal(tmpSphere, obb);
				tmpSphere.center -= normal;
				closest_point = tmpSphere.center;
				break;
			}
		}
		return closest_point;
	}

	return Vector3{ 0.0f ,0.0f ,0.0f };
}

Vector3 GetClosestPointOnOBB([[maybe_unused]] const Sphere& sphere0, [[maybe_unused]] const Vector3& velocity, [[maybe_unused]] const Sphere& sphere1) {
	Vector3 closest_point = { 0, 0, 0 };

	return closest_point;
}

Vector3 CalculateReflection(const Vector3& incoming, const Vector3& normal) {
	Vector3 n = Normalize(normal);
	float dot_product = Dot(incoming, n);
	Vector3 reflection = incoming - n * (2.0f * dot_product);
	return reflection;
}

EulerTransformData ExtractTransform(const Matrix4x4& matrix) {
	EulerTransformData data;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	// 位置の抽出
	position.x = matrix.m[3][0];
	position.y = matrix.m[3][1];
	position.z = matrix.m[3][2];

	// スケールの抽出
	scale.x = std::sqrt(matrix.m[0][0] * matrix.m[0][0] + matrix.m[1][0] * matrix.m[1][0] + matrix.m[2][0] * matrix.m[2][0]);
	scale.y = std::sqrt(matrix.m[0][1] * matrix.m[0][1] + matrix.m[1][1] * matrix.m[1][1] + matrix.m[2][1] * matrix.m[2][1]);
	scale.z = std::sqrt(matrix.m[0][2] * matrix.m[0][2] + matrix.m[1][2] * matrix.m[1][2] + matrix.m[2][2] * matrix.m[2][2]);

	// スケールを除去して回転成分を取り出す
	Matrix4x4 rotationMatrix = matrix;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (i == 0) {
				rotationMatrix.m[i][j] /= scale.x;
			}
			else if (i == 1) {
				rotationMatrix.m[i][j] /= scale.y;
			}
			else {
				rotationMatrix.m[i][j] /= scale.z;
			}
		}
	}

	// 回転行列からオイラー角を計算する
	rotation.y = std::asin(-rotationMatrix.m[0][2]);
	if (std::cos(rotation.y) > 0.0001) {
		rotation.x = std::atan2(rotationMatrix.m[1][2], rotationMatrix.m[2][2]);
		rotation.z = std::atan2(rotationMatrix.m[0][1], rotationMatrix.m[0][0]);
	}
	else {
		rotation.x = std::atan2(-rotationMatrix.m[2][1], rotationMatrix.m[1][1]);
		rotation.z = 0;
	}

	data.translate_ = position;
	data.rotate_ = rotation;
	data.scale_ = scale;

	return data;
}

int getDigits(int number, int position) {
	// 負の数の場合は正の数に変換
	number = abs(number);

	// 0番目から数える場合の対応
	for (int i = 0; i < position; ++i) {
		number /= 10;
	}

	return number % 10;  // 目的の桁の数値を取得}
}
