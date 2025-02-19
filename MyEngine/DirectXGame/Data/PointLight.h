#pragma once
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"

/// <summary>
/// PointLight.h
/// ポイントライト構造体のファイル
/// </summary>

struct PointLightData {
	int32_t type;
	float padding[3];
	Vector4 color;
	Vector3 position;
	float intensity;
	float radius;
	float decay;
	float padding2[2];
};