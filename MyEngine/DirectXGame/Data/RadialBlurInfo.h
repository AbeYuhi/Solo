#pragma once
#include <stdint.h>

/// <summary>
/// SmoothingInfo.h
/// ポストエフェクト「Smoothing」に必要な情報が入っているファイル
/// </summary>

struct RadialBlurInfo
{
    int32_t numSamples;
    float blurWidth;
};