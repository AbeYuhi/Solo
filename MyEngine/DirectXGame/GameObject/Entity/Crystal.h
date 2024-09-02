#pragma once
#include "Manager/CollisionManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"

struct CrystalData {
	std::shared_ptr<Model> model;
	RenderItem* renderItem;
	Collider* collider;
};

class Crystal
{
public:
	void Initialize(std::shared_ptr<Model> model,
		RenderItem* renderItem,
		Collider* collider);

	void Update();

	void Draw();

	inline void SetNumberofSlashAttacks(int* num) { numberofSlashAttacks_ = num; }
	inline void SetComboDestroyCount(int* num) { comboDestroyCount_ = num; }

private:

	CrystalData data_;
	bool isBreak_;
	int* numberofSlashAttacks_;
	int* comboDestroyCount_;
};

