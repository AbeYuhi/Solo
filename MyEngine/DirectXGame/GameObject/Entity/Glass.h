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
class GlassParticle;
class Player;

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
		//ボロノイズの生成
		std::unique_ptr<VoronoiSiteManager> voronoiSiteManager;
		//ガラス片を生成させるクラス
		std::unique_ptr<GlassParticle> glassParticle;
		bool isBreaked;
		bool isConnected;
		float breakTime;
		Vector3 velocity;
		//スコア
		static const int kScore = 100;
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

	/// <summary>
	/// プレイヤーをポインタでセット
	/// </summary>
	/// <param name="player"></param>
	inline void SetPlayer(Player* player) { player_ = player; }

private:

	Collider* mainColldier_;
	ModelDrawInfo mainInfo_;
	Player* player_;
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

};

class VoronoiSiteManager {
public:
	VoronoiSiteManager(Vector3* glassCenterPos, Vector3* glassSize);
	~VoronoiSiteManager() = default;

	/// <summary>
	/// 衝突地点を基準に新しいサイトを追加
	/// </summary>
	/// <param name="collisionPoint"></param>
	void AddSites();

	void SitesDraw();

	/// <summary>
	/// サイトの位置を取得するゲッター
	/// </summary>
	/// <returns></returns>
	const std::vector<Vector3>& GetSites() const {
		return sites_;
	}

private:
	std::shared_ptr<MyEngine::Model> model_;
	std::vector<Vector3> sites_; // 現在のサイト（ボロノイ分割用の点）
	std::vector<std::unique_ptr<ModelDrawInfo>> siteDatas_; // サイトの位置を知るためのテスト的なデータ
	Vector3* glassCenterPos_;
	Vector3* glassSize_;
	const int numNewSites_ = 7;
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
