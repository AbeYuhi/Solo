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

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model"></param>
	/// <param name="renderItem"></param>
	/// <param name="collider"></param>
	void Initialize(std::shared_ptr<MyEngine::Model> model,
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
	/// 残りプレイヤーの弾数
	/// </summary>
	/// <param name="num"></param>
	inline void SetNumberofSlashAttacks(int* num) { numberofSlashAttacks_ = num; }

	/// <summary>
	/// プレイヤーのコンボ数
	/// </summary>
	/// <param name="num"></param>
	inline void SetComboDestroyCount(int* num) { comboDestroyCount_ = num; }

	/// <summary>
	/// プレイヤーをポインタでセット
	/// </summary>
	/// <param name="player"></param>
	inline void SetPlayer(Player* player) { player_ = player; }

private:

	Player* player_;
	ModelDrawInfo info_;
	Collider* collider_;
	bool isBreak_;
	int* numberofSlashAttacks_;
	int* comboDestroyCount_;
};

