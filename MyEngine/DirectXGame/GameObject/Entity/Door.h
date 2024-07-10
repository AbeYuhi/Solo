#pragma once
#include "Manager/CollisionManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"

class Door
{
public:
	Door();
	~Door();

	void Initialize(Vector3 pos, Vector3 size);

	void Update();

	void Draw();

private:
	std::shared_ptr<Model> model_;
	RenderItem leftDoorInfo_;
	Collider leftDoorCollider_;
	RenderItem rightDoorInfo_;
	Collider rightDoorCollider_;
	RenderItem buttonInfo_;
	Collider buttonCollider_;

	Vector3 pos_;
	Vector3 size_;
	bool isOpen_;
};

