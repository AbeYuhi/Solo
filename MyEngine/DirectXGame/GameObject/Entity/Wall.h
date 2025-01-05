#pragma once
#include "Manager/CollisionManager.h"
#include "Manager/DrawManager.h"
#include "GameObject/Particle/Object/GlassPieceParticle/GlassPieceParticle.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"
#include "Data/LevelData.h"

/// <summary>
/// Walls.h
/// 壁に関する宣言ファイル
/// </summary>

class Wall
{
public:
	enum MoveType {
		DONTMOVE,
		ALTERNATE_LEFT_RIGHT,
		ALTERNATE_UP_DOWN,
		XROTATE,
		YROTATE,
		ZROTATE,
	};

	struct WallData {
		std::shared_ptr<MyEngine::Model> model;
		std::shared_ptr<MyEngine::RenderItem> renderItem;
		Collider* collider;
		MoveType moveType;
		float moveLimit;
		float moveSpeed;
		float rotateSpeed;
	};

public:
	Wall();
	~Wall();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model"></param>
	/// <param name="renderItem"></param>
	/// <param name="collider"></param>
	/// <param name="info"></param>
	void Initialize(std::shared_ptr<MyEngine::Model> model,
		std::shared_ptr<MyEngine::RenderItem> renderItem,
		Collider* collider,
		WallInfo info);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:

	/// <summary>
	/// 動かない時の処理
	/// </summary>
	void DontMove();

	/// <summary>
	/// 左右に動く時の処理
	/// </summary>
	void MoveAlternateLeftRight();

	/// <summary>
	/// 上下に動く時の処理
	/// </summary>
	void MoveAlternateUpDown();

	/// <summary>
	/// X軸で回転するときの処理
	/// </summary>
	void MoveXRotate();

	/// <summary>
	/// Y軸で回転するときの処理
	/// </summary>
	void MoveYRotate();

	/// <summary>
	/// Z軸で回転するときの処理
	/// </summary>
	void MoveZRotate();

private:
	WallData data_;
	EulerTransformData keepData_;
	bool isTurnAround_;
	ModelDrawInfo infos_[6];
};