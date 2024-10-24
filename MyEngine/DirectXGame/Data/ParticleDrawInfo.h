#pragma once
#include "DirectXGame/Data/ResourceHandles.h"
#include "DirectXGame/Data/MaterialInfo.h"

/// <summary>
/// ParticleForGPU.h
/// パーティクルの描画をするために必要なデータを格納しているファイル
/// </summary>

struct ParticleDrawInfo {
	void Initialize(const ResourceHandles* srvHandle, const ParticleMaterialInfo* materialInfo, const int* kMaxParticleCount) {
		srvHandle_ = srvHandle;
		materialInfo_ = materialInfo;
		kMaxParticleCount_ = kMaxParticleCount;
	}
	const ResourceHandles* srvHandle_;
	const ParticleMaterialInfo* materialInfo_;
	const int* kMaxParticleCount_;
};