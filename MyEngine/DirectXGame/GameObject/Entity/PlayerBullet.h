#pragma once
#include <memory>
#include "Math/Math.h"
#include "Manager/InputManager.h"
#include "Manager/CollisionManager.h"
#include "Manager/TextureManager.h"
#include "Manager/DrawManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"
#include "Data/Collider.h"
#include "GameObject/Particle/Object/GlassPieceParticle/GlassPieceParticle.h"

/// <summary>
/// PlayerBullet.h
/// プレイヤーの弾を処理しているファイル
/// </summary>

class PlayerBullet
{
public:
	PlayerBullet() = default;
	~PlayerBullet() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="mousePos"></param>
	void Initialize(Vector2 mousePos);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾が残り続ける残り時間
	/// </summary>
	/// <returns></returns>
	inline float GetLifeTime() { return lifeTime_; }

private:
	MyEngine::InputManager* input_;
	ModelDrawInfo info_;
	Collider collider_;

	float lifeTime_;
	Vector3 velocity_;
	Vector3 reticlePos_;
	float speed_;

	bool isGround_;
};

