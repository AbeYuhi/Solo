#pragma once
#include "Manager/CollisionManager.h"
#include "Manager/DrawManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"
#include "GameObject/Entity/Player.h"

/// <summary>
/// Crystal.h
/// クリスタルに関するファイル
/// </summary>

class Crystal
{
public:
	void Initialize(std::shared_ptr<MyEngine::Model> model,
		std::shared_ptr<MyEngine::RenderItem> renderItem,
		Collider* collider);

	void Update();

	void Draw();

	inline void SetNumberofSlashAttacks(int* num) { numberofSlashAttacks_ = num; }
	inline void SetComboDestroyCount(int* num) { comboDestroyCount_ = num; }
	inline void SetPlayer(Player* player) { player_ = player; }

private:

	Player* player_;
	ModelDrawInfo info_;
	Collider* collider_;
	bool isBreak_;
	int* numberofSlashAttacks_;
	int* comboDestroyCount_;
};

