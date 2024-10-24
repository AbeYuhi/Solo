#pragma once
#include <stdint.h>

/// <summary>
/// SmoothingInfo.h
/// ポストエフェクト「Smoothing」に必要な情報が入っているファイル
/// </summary>

struct SmoothingInfo {
	int32_t kernelSize;
	int32_t type;
	float blurStrength;
};