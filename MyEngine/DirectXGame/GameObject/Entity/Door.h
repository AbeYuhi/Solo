#pragma once
#include "Manager/CollisionManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"

class Door
{
public:
	Door();
	~Door();

	void Initialize();

	void Update();

	void Draw();

private:
	std::shared_ptr<Model> model_;
	RenderItem leftDoorInfo_;
	Collider leftDoorCollider_;
	RenderItem rightDoorInfo_;
	Collider rightDoorCollider_;

};

