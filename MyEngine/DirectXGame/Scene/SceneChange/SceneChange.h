#pragma once
#include "Object/Sprite.h"
#include "Data/SpriteItem.h"
#include "Data/SpriteData.h"
#include "GameObject/Camera/SpriteCamera.h"
#include "Manager/DrawManager.h"

/// <summary>
/// SceneChange.h
/// 画面遷移
/// </summary>

class SceneChange
{
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static SceneChange* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Inialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// シーンチェンジをスタート
	/// </summary>
	void StartSceneChange();

	inline bool IsChange() const { return isChange_; }
	inline bool IsStart() const { return isStart_; }

	inline void SetScore(int score) { gameScore_ = score; }
	inline int GetScore() const { return gameScore_; }

private:
	SceneChange() = default;
	~SceneChange() = default;

	SpriteCamera* spriteCamera_;
	SpriteDrawInfo whiteTextureInfo_;

	bool isStart_;
	bool isReverse_;
	bool isChange_;
	int gameScore_;
	float time_;
};

