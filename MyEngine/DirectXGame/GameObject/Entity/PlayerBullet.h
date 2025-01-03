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

	void Initialize(Vector2 mousePos);

	void Update();

	void Draw();

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

