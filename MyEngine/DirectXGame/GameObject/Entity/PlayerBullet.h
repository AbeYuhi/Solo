#pragma once
#include <memory>
#include "Manager/InputManager.h"
#include "Manager/CollisionManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"
#include "Data/Collider.h"

class PlayerBullet
{
public:
	PlayerBullet() = default;
	~PlayerBullet() = default;

	void Initialize();

	void Update();

	void Draw();

private:
	InputManager* input_;
	std::shared_ptr<Model> model_;
	RenderItem renderItem_;
	Collider collider_;

	float lifeTime_;
	Vector3 velocity_;
	Vector3 reticlePos_;

};

