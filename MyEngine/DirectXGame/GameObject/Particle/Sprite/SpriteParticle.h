#pragma once
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Object/Sprite.h"

/// <summary>
/// SpriteParticle.h
/// スプライト上のパーティクルをテストしているクラス
/// </summary>

class SpriteParticle : public MyEngine::ParticleManager
{
public:

	SpriteParticle(int maxParticleCount);
	~SpriteParticle() = default;

	void Initialize() override;

	void Update() override;

	void EmitterDraw() override;

	void Draw() override;

private: //メンバ関数

	ParticleInfo MakeNewParticle() override;

private:
	std::shared_ptr<MyEngine::Sprite> particleSprite_;
	SpriteData spriteData_;
};