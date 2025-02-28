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

	/// <summary>
	/// ガラスの大きさのセッター
	/// </summary>
	/// <param name="size"></param>
	void SetGlassSize(Vector3 size) { glassSize_ = size; }

	/// <summary>
	/// エミッターのセッター
	/// </summary>
	/// <param name="emitter"></param>
	void SetEmitter(Emitter emitter) { emitter_ = emitter; }

	/// <summary>
	/// エミッターの取得
	/// </summary>
	/// <param name="size"></param>
	Emitter* GetEmitterPointer() { return &emitter_; }

private: //メンバ関数

	/// <summary>
	/// パーティクルの粒子を生成する処理
	/// </summary>
	/// <returns></returns>
	ParticleInfo MakeNewParticle() override;

private:
	std::shared_ptr<MyEngine::Model> particleModel_;

	Vector3 glassSize_;
};

