#pragma once
#include "Manager/CollisionManager.h"
#include "Manager/DrawManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"

/// <summary>
/// Door.h
/// 障害物であるドアに関するファイル
/// </summary>

struct DoorData {
	std::shared_ptr<MyEngine::Model> model;
	MyEngine::RenderItem* renderItem;
	Collider* collider;
};

class Door
{
public:
	Door();
	~Door();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model"></param>
	/// <param name="renderItem"></param>
	/// <param name="collider"></param>
	void Initialize(
		std::shared_ptr<MyEngine::Model> model,
		std::shared_ptr<MyEngine::RenderItem> renderItem,
		Collider* collider);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 右側の扉のセット
	/// </summary>
	/// <param name="model"></param>
	/// <param name="renderItem"></param>
	/// <param name="collider"></param>
	void SetRightDoor(std::shared_ptr<MyEngine::Model> model,
		std::shared_ptr<MyEngine::RenderItem> renderItem,
		Collider* collider);

	/// <summary>
	/// 左側の扉のセット
	/// </summary>
	/// <param name="model"></param>
	/// <param name="renderItem"></param>
	/// <param name="collider"></param>
	void SetLeftDoor(std::shared_ptr<MyEngine::Model> model,
		std::shared_ptr<MyEngine::RenderItem> renderItem,
		Collider* collider);

	/// <summary>
	/// ボタンに弾がヒットしたかを取得
	/// </summary>
	/// <param name="isHit"></param>
	inline void IsHitBullet(bool isHit) { isOpen_ = isHit; }

	/// <summary>
	/// プレイヤーの残り玉数
	/// </summary>
	/// <param name="num"></param>
	inline void SetNumberofSlashAttacks(int* num) { numberofSlashAttacks_ = num; }

	/// <summary>
	/// プレイヤーのコンボ数
	/// </summary>
	/// <param name="num"></param>
	inline void SetComboDestroyCount(int* num) { comboDestroyCount_ = num; }

private:

	ModelDrawInfo buttonInfo_;
	ModelDrawInfo leftDoorInfo_;
	ModelDrawInfo rightDoorInfo_;
	Collider* buttonCollider_;
	Collider* leftDoorCollider_;
	Collider* rightDoorCollider_;
	bool isOpen_;
	float moveTime_;

	uint32_t openDoorButtonTexture_;

	int* numberofSlashAttacks_;
	int* comboDestroyCount_;
};