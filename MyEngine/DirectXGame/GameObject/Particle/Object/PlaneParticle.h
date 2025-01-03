#pragma once
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Object/Model.h"

/// <summary>
/// PlaneParticle.h
/// 3Dオブジェクトのパーティクルをテストしているクラス
/// </summary>

class PlaneParticle : public MyEngine::ParticleManager
{
public:

	PlaneParticle(int maxParticleCount);
	~PlaneParticle() = default;

	void Initialize() override;

	void Update() override;

	void EmitterDraw() override;

	void Draw() override;

private: //メンバ関数

	ParticleInfo MakeNewParticle() override;

private:
	std::shared_ptr<MyEngine::Model> particleModel_;

	//風フィールド
	AccelerationField accelerationField_;

	//風が吹いているか
	bool isAccelerationField_;
};

