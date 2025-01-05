#pragma once
#include <memory>
#include <list>
#include "Object/Sprite.h"
#include "Manager/PostEffectManager.h"
#include "Manager/RandomManager.h"
#include "Manager/DrawManager.h"
#include "PlayerBullet.h"

/// <summary>
/// Player.h
/// プレイヤーに関する処理をしているファイル
/// </summary>

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
	/// 射出しているかをセット
	/// </summary>
	/// <param name="isShot"></param>
	inline void SetIsShot(bool isShot) { isShot_ = isShot; }

private:
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	int numberofSlashAttacks_;
	float gameOverTime_;
	bool isGameOver_;
	bool isGameClear_;
	int digitCount_;
	int comboDestroyCount_;

	uint32_t numberSpriteTextures_[10];
	SpriteDrawInfo infos_[3];

	MyEngine::RenderItem renderItem_;
	EulerTransformData* cameraData_;
	Collider collider_;

	float doorInvincibilityTime_;
	float glassInvincibilityTime_;

	bool isShot_;

	bool isHitEffect_;
	float time_;
};

