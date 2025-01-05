#pragma once
#include <stdint.h>
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"

/// <summary>
/// SpriteData.h
/// スプライトデータ構造体が入っているファイル
/// </summary>

struct SpriteData {
	uint32_t textureHandle_;
	Vector2 size_;
	Vector2 anchorPoint_;
	Vector2 baseUvPos_;
	Vector2 texSize_;
	bool isFlipX_;
	bool isFlipY_;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="textureHandle"></param>
	/// <param name="size"></param>
	/// <param name="anchorPoint"></param>
	/// <param name="baseUvPos"></param>
	/// <param name="texSize"></param>
	/// <param name="isFlipX"></param>
	/// <param name="isFlipY"></param>
	void Initialize(uint32_t textureHandle = 0, Vector2 size = { 640, 360 }, Vector2 anchorPoint = { 0.5f, 0.5f }, Vector2 baseUvPos = { 0, 0 }, Vector2 texSize = { 1.0f, 1.0f }, bool isFlipX = false, bool isFlipY = false);
};