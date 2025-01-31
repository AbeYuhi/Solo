#pragma once
#include <cstdint>
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"

/// <summary>
/// Material.h
/// マテリアル情報をまとめているファイル
/// </summary>

struct Material {
	Vector4 color;
	int32_t enableLightint;
	int32_t isSpecularReflection;
	int32_t isEnvironment;
	float padding;
	Matrix4x4 uvTransform;
	float shininess;
	Vector3 shininessColor;
	float environmentCoefficient;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();
};

struct ParticleMaterial {
	Vector4 color;
	Matrix4x4 uvTransform;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();
};