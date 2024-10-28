#pragma once
#include "Math/Vector4.h"

/// <summary>
/// VignettingInfo.h
/// Vignettingのポストエフェクトの際にGPUに送るデータ
/// </summary>

struct VignettingInfo
{
    Vector4 color;
};