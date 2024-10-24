#pragma once
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Vector4.h"

/// <summary>
/// ParticleForGPU.h
/// パーティクルの情報をGPUに送るデータを格納するためのファイル
/// </summary>

struct ParticleForGPU {
	Matrix4x4 WVP_;
	Matrix4x4 World_;
	Vector4 color_;
};