#pragma once
#include "DirectXGame/Data/ResourceHandles.h"
#include "DirectXGame/Data/MaterialInfo.h"

/// <summary>
/// ParticleForGPU.h
/// パーティクルの描画をするために必要なデータを格納しているファイル
/// </summary>

struct ParticleDrawInfo {
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="srvHandle"></param>
	/// <param name="materialInfo"></param>
	/// <param name="kMaxParticleCount"></param>
	void Initialize(const ResourceHandles* srvHandle, MyEngine::ParticleMaterialInfo* materialInfo, const int* kMaxParticleCount) {
		srvHandle_ = srvHandle;
		materialInfo_ = materialInfo;
		kMaxParticleCount_ = kMaxParticleCount;
	}
	const ResourceHandles* srvHandle_;
	MyEngine::ParticleMaterialInfo* materialInfo_;
	const int* kMaxParticleCount_;
};