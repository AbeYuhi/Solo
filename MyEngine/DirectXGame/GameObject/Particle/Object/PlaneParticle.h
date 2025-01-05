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
	std::shared_ptr<MyEngine::Model> particleModel_;

	//風フィールド
	AccelerationField accelerationField_;

	//風が吹いているか
	bool isAccelerationField_;
};

