#pragma once

/// <summary>
/// VignetteBlurInfo.h
/// VignetteBlurInfoのポストエフェクトの際にGPUに送るデータ
/// </summary>

struct VignetteBlurInfo
{
    float intensity; // 赤みの強度調整用
    float blurAmount; // ぼかしの強度調整用
};