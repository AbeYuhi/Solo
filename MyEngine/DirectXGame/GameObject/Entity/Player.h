#pragma once
#include <memory>
#include <list>
#include "Object/Sprite.h"
#include "Manager/PostEffectManager.h"
#include "Manager/RandomManager.h"
#include "Manager/DrawManager.h"
#include "Manager/AudioManager.h"
#include "PlayerBullet.h"

/// <summary>
/// Player.h
/// プレイヤーに関する処理をしているファイル
/// </summary>

class ShotStateManager;

class Player
{
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="cameraData"></param>
	void Initialize(EulerTransformData* cameraData);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾を発射する処理
	/// </summary>
	/// <param name="bulletPos">弾を発射する座標</param>
	void Shoot(Vector2 bulletPos);

	/// <summary>
	/// カメラデータをセット
	/// </summary>
	/// <param name="data"></param>
	inline void SetCameraData(EulerTransformData* data) { cameraData_ = data; }

	/// <summary>
	/// ゲームオーバーか取得
	/// </summary>
	/// <returns></returns>
	inline bool IsGameOver() const { return isGameOver_; }

	/// <summary>
	/// ゲームクリアか取得
	/// </summary>
	/// <returns></returns>
	inline bool IsGameClear() const { return isGameClear_; }

	/// <summary>
	/// 残り玉数の取得
	/// </summary>
	/// <returns></returns>
	inline int* GetNumberofSlashAttacks() { return &numberofSlashAttacks_; }

	/// <summary>
	/// コンボ数の取得
	/// </summary>
	/// <returns></returns>
	inline int* GetComboDestroyCount() { return &comboDestroyCount_; }

	/// <summary>
	/// ボールをロストしたか
	/// </summary>
	/// <returns></returns>
	inline bool IsBallLost() const { return isBallLost_; }

	/// <summary>
	/// 射出しているかをセット
	/// </summary>
	/// <param name="isShot"></param>
	inline void SetIsShot(bool isShot) { isShot_ = isShot; }

	/// <summary>
	/// スコアを加算する関数
	/// </summary>
	/// <param name="score"></param>
	inline void AddScore(int score) { score_ += score; }

	inline int GetScore() const {
		return score_;
	}

private:
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	int numberofSlashAttacks_;
	float gameOverTime_;
	bool isGameOver_;
	bool isGameClear_;
	bool isBallLost_;
	int digitCount_;
	int comboDestroyCount_;

	uint32_t numberSpriteTextures_[10];
	SpriteDrawInfo infos_[3];

	uint32_t bulletNumTextures_[5];
	SpriteDrawInfo bulletNumInfo_;

	uint32_t bulletComboTextures_;
	SpriteDrawInfo bulletComboInfo_[9];

	MyEngine::RenderItem renderItem_;
	EulerTransformData* cameraData_;
	Collider collider_;

	float doorInvincibilityTime_;
	float glassInvincibilityTime_;

	//球の射出処理
	std::unique_ptr<ShotStateManager> shotStateManager_;

	bool isShot_;
	bool isHitEffect_;
	float time_;

	uint32_t missSound_;
	const float kMissSoundVolume_ = 1.0f;
	const float kGameOverTimeLimit = 2.0f;

	int score_;

public:
	//コンボがいくつで上昇するのかの指標
	const int kComboIncreaseStep = 10;
	const int kBallLossPerHit = 10;
};

enum class ShotStateType {
	State1,
	State2,
	State3,
	State4,
	State5
};

class ShotState {
public:
	virtual ~ShotState() = default;
	virtual void Shoot(Player* player) = 0;
	virtual std::unique_ptr<ShotState> NextState() = 0;
	virtual ShotStateType stateNo() = 0;

protected:
	const int kBallSpacing = 30;
};

class ShotState1 : public ShotState {
public:
	void Shoot(Player* player) override;
	std::unique_ptr<ShotState> NextState() override;
	ShotStateType stateNo() override;
};

class ShotState2 : public ShotState {
public:
	void Shoot(Player* player) override;
	std::unique_ptr<ShotState> NextState() override;
	ShotStateType stateNo() override;
};

class ShotState3 : public ShotState {
public:
	void Shoot(Player* player) override;
	std::unique_ptr<ShotState> NextState() override;
	ShotStateType stateNo() override;
};

class ShotState4 : public ShotState {
public:
	void Shoot(Player* player) override;
	std::unique_ptr<ShotState> NextState() override;
	ShotStateType stateNo() override;
};

class ShotState5 : public ShotState {
public:
	void Shoot(Player* player) override;
	std::unique_ptr<ShotState> NextState() override;
	ShotStateType stateNo() override;
};

class ShotStateManager {
public:
	ShotStateManager(Player* player);

	~ShotStateManager() = default;

	/// <summary>
	/// コンボが上昇したときにプレイヤーの
	/// </summary>
	void IncrementCombo();

	/// <summary>
	/// 弾の射出のための処理
	/// </summary>
	/// <param name="player"></param>
	void Shoot();

private:
	Player* player_;
	std::unique_ptr<ShotState> currentState_;
	uint32_t comboSound_;
	const float kComboSoundVolume_ = 0.8f;
	uint32_t shotSound_;
	const float kShotSoundVolume_ = 0.7f;
	int lastProcessedCombo_;

};