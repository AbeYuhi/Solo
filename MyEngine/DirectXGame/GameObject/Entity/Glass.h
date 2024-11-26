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
		bool isBreaked;
		bool isConnected;
		float breakTime;
		Vector3 velocity;
	};

public:
	Glass();
	~Glass();

	void Initialize(std::shared_ptr<MyEngine::Model> model,
		MyEngine::RenderItem* renderItem,
		GlassInfo info);

	void Update();

	void Draw();

	void ParticleDraw();

private:

	void DontMoveGlass();

	void MoveGlassUpRight();

	void MoveGlassAlternateLeftRight();


private:

	std::shared_ptr<MyEngine::Model> model_;
	MyEngine::RenderItem renderItem_;
	Collider mainColldier_;
	
	MoveType type_;
	GroundingInfo groudingInfo_;
	unsigned int divisionX_;
	unsigned int divisionY_;
	std::vector<std::vector<std::unique_ptr<MyEngine::RenderItem>>> renderItems_;
	std::vector<std::vector<GlassPiece>> colliders_;

	bool isBreak;
	float time_;
	EulerTransformData keepData_;
	Vector3 moveLimit_;
	bool isTurnAround_;

	//分割片のサイズを計算
	float segmentWidth_;
	float segmentHeight_;

	//基準点（ガラス全体の中心点）
	Vector3 base_;
	//分割片ガラスのサイズ
	Vector3 size_;

	//
};

