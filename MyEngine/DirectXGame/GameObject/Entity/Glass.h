#pragma once
#include "Manager/AudioManager.h"
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

class GlassMove;

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

	EulerTransformData* GetModelData() { return &mainInfo_.renderItem->worldTransform_.data_; }

	EulerTransformData GetKeepData() const { return keepData_; }
	Vector3 GetBaseData() const { return base_; }
	float GetMoveSpeed() const { return moveSpeed_; }
	Vector3 GetMoveLimit() const { return moveLimit_; }

private:

	Collider* mainColldier_;
	ModelDrawInfo mainInfo_;
	
	MoveType type_;
	std::unique_ptr<GlassMove> moveState_;
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

	//ガラスのサウンド
	uint32_t glassSound_;
};

class MoveState {
public:
	virtual ~MoveState() = default;
	virtual void Move(Glass* glass) = 0;
};

class DontMove : public MoveState{
public:
	void Move([[maybe_unused]] Glass* glass) override;

private:
	float time_;
};

class UpRight : public MoveState {
public:
	void Move(Glass* glass) override;

private:
	float time_;
};

class AlternateLeftRight : public MoveState {
public:
	void Move(Glass* glass) override;

private:
	float time_;
	bool isTurnAround_;
};

class AlternateUpDown : public MoveState {
public:
	void Move(Glass* glass) override;

private:
	float time_;
	bool isTurnAround_;
};

class GlassMove {
public:
	GlassMove(Glass* glass, Glass::MoveType type);
	~GlassMove();

	void Move();

private:
	Glass* glass_;
	std::unique_ptr<MoveState> state_;
};
