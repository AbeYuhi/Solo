#pragma once
#include "Manager/CollisionManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"

struct DoorData {
	std::shared_ptr<Model> model;
	RenderItem* renderItem;
	Collider* collider;
};

class Door
{
public:
	Door();
	~Door();

	void Initialize(
		std::shared_ptr<Model> model,
		RenderItem* renderItem,
		Collider* collider);

	void Update();

	void Draw();

	void SetRightDoor(std::shared_ptr<Model> model,
		RenderItem* renderItem,
		Collider* collider);
	void SetLeftDoor(std::shared_ptr<Model> model,
		RenderItem* renderItem,
		Collider* collider);
	inline void IsHitBullet(bool isHit) { isOpen_ = true; }

	inline void SetNumberofSlashAttacks(int* num) { numberofSlashAttacks_ = num; }
	inline void SetComboDestroyCount(int* num) { comboDestroyCount_ = num; }

private:

	DoorData button_;
	DoorData leftDoor_;
	DoorData rightDoor_;
	bool isOpen_;
	float moveTime_;

	int* numberofSlashAttacks_;
	int* comboDestroyCount_;
};