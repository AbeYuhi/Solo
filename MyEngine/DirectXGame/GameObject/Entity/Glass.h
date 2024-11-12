#pragma once
#include "Manager/CollisionManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"
#include "Data/LevelData.h"

/// <summary>
/// Glass.h
/// 障害物であるガラスに関するファイル
/// </summary>

class Glass
{
public:
	enum MoveType {
		DONTMOVE,
		ALTERNATE_LEFT_RIGHT,
		UPRIGHT
	};
	struct GroundingInfo {
		bool up;
		bool down;
		bool rigft;
		bool left;
	};

	struct GlassPiece {
		std::unique_ptr<Collider> collider;
		bool isBreak;
	};

public:
	Glass();
	~Glass();

	void Initialize(std::shared_ptr<Model> model,
		RenderItem* renderItem,
		GlassInfo info);

	void Update();

	void Draw();


private:

	std::shared_ptr<Model> model_;
	RenderItem renderItem_;
	Collider mainColldier_;
	
	MoveType type_;
	GroundingInfo groudingInfo_;
	unsigned int divisionX_;
	unsigned int divisionY_;
	std::vector<std::vector<std::unique_ptr<RenderItem>>> renderItems_;
	std::vector<std::vector<std::unique_ptr<Collider>>> colliders_;

	bool isBreak;
};

