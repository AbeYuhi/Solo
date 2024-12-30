#pragma once
#include "Manager/CollisionManager.h"
#include "Manager/DrawManager.h"
#include "GameObject/Particle/Object/GlassPieceParticle/GlassPieceParticle.h"
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
		ALTERNATE_UP_DOWN,
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
		std::unique_ptr<GlassPieceParticle> particle;
		Emitter emitter;
		bool isBreaked;
		bool isConnected;
		float breakTime;
		Vector3 velocity;
	};

public:
	Glass();
	~Glass();

	void Initialize(std::shared_ptr<MyEngine::Model> model,
		std::shared_ptr<MyEngine::RenderItem> renderItem,
		Collider* collider,
		GlassInfo info);

	void Update();

	void Draw();

private:

	void DontMoveGlass();

	void MoveGlassUpRight();

	void MoveGlassAlternateLeftRight();

	void MoveGlassAlternateUpDown();


private:

	Collider* mainColldier_;
	ModelDrawInfo mainInfo_;
	
	MoveType type_;
	GroundingInfo groudingInfo_;
	unsigned int divisionX_;
	unsigned int divisionY_;
	std::vector<std::vector<std::unique_ptr<ModelDrawInfo>>> infos_;
	std::vector<std::vector<GlassPiece>> colliders_;

	bool isBreak;
	float time_;
	EulerTransformData keepData_;
	Vector3 moveLimit_;
	float moveSpeed_;
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

