#pragma once
#include "DirectXGame/Manager/TextureManager.h"

/// <summary>
/// IScene.h
/// シーンの親クラス
/// </summary>

enum GameScene {
	TITLE, INGAME, MENU, GAMEOVER, GAMECLEAR, SCENENUM
};

class IScene
{
public:
	IScene() = default;
	virtual ~IScene() = default;

	/// <summary>
	/// シーンの初期化関数
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// シーンのアップデート関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// シーンの描画処理
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// シーン削除時の処理
	/// </summary>
	virtual void Finalize() = 0;

	inline int GetSceneNo() { return sceneNo_; }
	inline int GetPreSceneNo() { return preSceneNo_; }

	//画像ハンドル
	static uint32_t uvCheckerHandle_;
	static uint32_t whiteTextureHandle_;

protected:
	static bool debugMode_;
	static int sceneNo_;
	static int preSceneNo_;
};