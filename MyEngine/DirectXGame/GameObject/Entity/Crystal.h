#pragma once
#include "Manager/CollisionManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"
#include "GameObject/Entity/Player.h"

/// <summary>
/// Crystal.h
/// クリスタルに関するファイル
/// </summary>

struct CrystalData {
	std::shared_ptr<MyEngine::Model> model;
	MyEngine::RenderItem* renderItem;
	Collider* collider;
};

class Crystal
{
public:
	void Initialize(std::shared_ptr<MyEngine::Model> model,
		MyEngine::RenderItem* renderItem,
		Collider* collider);

	void Update();

	void Draw();

	void DrawTransparentObject();

	inline void SetNumberofSlashAttacks(int* num) { numberofSlashAttacks_ = num; }
	inline void SetComboDestroyCount(int* num) { comboDestroyCount_ = num; }
	inline void SetPlayer(Player* player) { player_ = player; }

private:

	Player* player_;
	CrystalData data_;
	bool isBreak_;
	int* numberofSlashAttacks_;
	int* comboDestroyCount_;
};

