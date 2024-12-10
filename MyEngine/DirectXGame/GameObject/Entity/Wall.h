#pragma once
#include "Manager/CollisionManager.h"
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
		MyEngine::RenderItem* renderItem;
		Collider* collider;
		MoveType moveType;
		float moveLimit;
		float moveSpeed;
		float rotateSpeed;
	};

public:
	Wall();
	~Wall();

	void Initialize(std::shared_ptr<MyEngine::Model> model,
		MyEngine::RenderItem* renderItem,
		Collider* collider,
		WallInfo info);

	void Update();

	void Draw();

private:

	void DontMove();

	void MoveAlternateLeftRight();

	void MoveAlternateUpDown();

	void MoveXRotate();
	void MoveYRotate();
	void MoveZRotate();

private:
	WallData data_;
	float time_;
	EulerTransformData keepData_;
	bool isTurnAround_;
	Vector3 basePos_;
	MyEngine::RenderItem renderItems_[6];
};