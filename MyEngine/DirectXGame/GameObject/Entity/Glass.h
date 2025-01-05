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
		GlassInfo info);

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
	/// ガラスが動かない時の処理
	/// </summary>
	void DontMoveGlass();

	/// <summary>
	/// ガラスが起き上がるときの処理
	/// </summary>
	void MoveGlassUpRight();

	/// <summary>
	/// ガラスが左右に動く時の処理
	/// </summary>
	void MoveGlassAlternateLeftRight();

	/// <summary>
	/// ガラスが上下に動く時の処理
	/// </summary>
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

