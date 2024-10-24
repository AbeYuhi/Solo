#pragma once
#include "DirectXGame/Math/Vector3.h"

/// <summary>
/// CameraForGPU.h
/// カメラの位置情報をGPUに送るための構造体があるファイル
/// </summary>

struct CameraForGPU {
	Vector3 worldPosition;
};