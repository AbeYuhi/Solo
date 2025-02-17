#pragma once
#include "IScene.h"
#include "DirectXGame/GameObject/Camera/TitleCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"
#include "Manager/PostEffectManager.h"
#include "Manager/DrawManager.h"
#include "Manager/InputManager.h"
#include "Scene/SceneChange/SceneChange.h"
#include "Data/SpriteItem.h"
#include "Object/Model.h"
#include "Object/Sprite.h"
#include "LoadFile/LevelScene.h"

/// <summary>
/// TitleScene.h
/// タイトルシーン
/// </summary>

class TitleScene : public IScene
{
public:
	TitleScene();
	~TitleScene();

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
	MyEngine::PostEffectManager* postEffectManager_;

	std::unique_ptr<TitleCamera> titleCamera_;

	MyEngine::CollisionManager* collisionManager_;

	SpriteDrawInfo leftMouseClickInfo_;
	SpriteDrawInfo titleNameInfo_;
	SpriteDrawInfo backGroundInfo_;
	SpriteDrawInfo scoreInfo_;

	uint32_t scoreTexture_;
	uint32_t clearTexture_;

	SceneChange* sceneChange_;

	//ライト
	MyEngine::LightObjectManager* lightObj_;

	//ゲームbgm
	uint32_t bgmIndex_;

	float time_;
	std::unique_ptr<LevelScene> levelScene_;
	std::unique_ptr<LevelScene> levelScene1_;
	std::unique_ptr<LevelScene> levelScene2_;

	float changeTime_;
	bool change_;

	bool isPreviousSceneInGame_;
	bool isResult_;
	bool isScoreImageScale_;

	bool isGameClear_;
};