#pragma once
#include "IScene.h"
#include "Object/Sprite.h"
#include "DirectXGame/GameObject/Camera/GameClearCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"

/// <summary>
/// GameClear.h
/// ゲームクリアシーン
/// </summary>

class GameClearScene : public IScene
{
public:
	GameClearScene();
	~GameClearScene();

	/// <summary>
	/// シーンの初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// シーンのアップデート関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// シーンの描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// シーン削除時の処理
	/// </summary>
	void Finalize() override;

private:
	MainCamera* mainCamera_;
	SpriteCamera* spriteCamera_;

	uint32_t leftMouseClickTexture_;
	std::shared_ptr<MyEngine::Sprite> leftMosueClickSprite_;
	MyEngine::SpriteItem leftMouseClickInfo_;

	uint32_t gameOverTexture_;
	std::shared_ptr<MyEngine::Sprite> gameOverSprite_;
	MyEngine::SpriteItem gameOverInfo_;
};