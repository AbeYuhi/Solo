#pragma once
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/AABB.h"

/// <summary>
/// AccelerationField.h
/// パーティクル時にエリアにパーティクルに力を加える情報を格納するための構造体があるファイル
/// </summary>

struct AccelerationField {
	Vector3 accelerationField;
	AABB area;
};