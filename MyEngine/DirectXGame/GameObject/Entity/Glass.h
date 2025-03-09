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
class VoronoiSiteManager;

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
	const float kGlassSoundVolume_ = 0.7f;

	//ボロノイズの生成
	std::unique_ptr<VoronoiSiteManager> voronoiSiteManager_;
};

class VoronoiSiteManager {
public:
	VoronoiSiteManager(Vector3* glassCenterPos,	float* glassWidth, float* glassHeight) : glassCenterPos_(glassCenterPos), glassWidth_(glassWidth), glassHeight_(glassHeight), numNewSites_(15){}
	~VoronoiSiteManager() = default;

	// 衝突地点を基準に新しいサイトを追加
	void addSites(Vector3 collisionPoint) {
		for (int i = 0; i < numNewSites_; ++i) {
			// 0.3m（玉のサイズ）より少し大きめの範囲でサイトを生成
			float minRadius = 0.3f; // 最小半径（玉より少し大きい）
			float r = minRadius + (kMaxRadius - minRadius) * sqrt(MyEngine::RandomManager::GetInstance()->GetRandomNumber(0.0f, 1.0f));
			float theta = MyEngine::RandomManager::GetInstance()->GetRandomNumber(0.0f, 1.0f) * 2.0f * M_PI; // 水平方向の角度
			float phi = acos(1.0f - 2.0f * MyEngine::RandomManager::GetInstance()->GetRandomNumber(0.0f, 1.0f)); // 垂直方向の角度

			// 球面座標を直交座標(x, y, z)に変換
			float x = collisionPoint.x + r * sin(phi) * cos(theta);
			float y = collisionPoint.y + r * sin(phi) * sin(theta);
			float z = collisionPoint.z;

			// ガラスの範囲内に収まるように制限
			if (x < glassCenterPos_->x - *glassWidth_ / 2 || x > glassCenterPos_->x + *glassWidth_ / 2 ||
				y < glassCenterPos_->y - *glassHeight_ / 2 || y > glassCenterPos_->y + *glassHeight_ / 2) {
				continue; // 範囲外のサイトは追加しない
			}

			// 生成したサイトをリストに追加
			sites_.push_back({ x, y, z });
		}
	}

private:
	std::vector<Vector3> sites_; // 現在のサイト（ボロノイ分割用の点）
	const float kMaxRadius = 0.4f; // サイトが生成される最大半径
	Vector3* glassCenterPos_;
	float* glassWidth_;
	float* glassHeight_;
	int numNewSites_;
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
