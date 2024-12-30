#pragma once
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Object/Model.h"

/// <summary>
/// GlassPieceParticle.h
/// ガラスの破壊演出をしているクラス
/// </summary>

class GlassPieceParticle : public MyEngine::ParticleManager
{
public:

	GlassPieceParticle(int maxParticleCount);
	~GlassPieceParticle() = default;

	void Initialize() override;

	void Update() override;

	void EmitterDraw() override;

	void Draw() override;

	void SetGlassSize(Vector3 size) { glassSize_ = size; }

	void SetEmitter(Emitter emitter) { emitter_ = emitter; }

private: //メンバ関数

	ParticleInfo MakeNewParticle() override;

private:
	std::shared_ptr<MyEngine::Model> particleModel_;

	Vector3 glassSize_;
};

