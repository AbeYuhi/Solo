#pragma once
#include <memory>
#include "Manager/InputManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"
#include "Data/Collider.h"

class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();

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

};
