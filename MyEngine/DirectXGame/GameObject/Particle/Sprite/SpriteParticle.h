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

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// エミッターの描画処理
	/// </summary>
	void EmitterDraw() override;

	/// <summary>
	/// パーティクルの描画処理
	/// </summary>
	void Draw() override;

private: //メンバ関数
	
	/// <summary>
	/// パーティクルの粒子を生成する処理
	/// </summary>
	/// <returns></returns>
	ParticleInfo MakeNewParticle() override;

private:
	std::shared_ptr<MyEngine::Sprite> particleSprite_;
	SpriteData spriteData_;
};