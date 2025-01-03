#pragma once
#include "IScene.h"
#include "DirectXGame/GameObject/Camera/TitleCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"
#include "Manager/PostEffectManager.h"
#include "Manager/DrawManager.h"
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

	void Initialize() override;

	void Update() override;

	void Draw() override;

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

	float time_;
	LevelScene levelScene_;

	float changeTime_;
	bool change_;

	bool isPreviousSceneInGame_;
	bool isResult_;
	bool isScoreImageScale_;

	bool isGameClear_;
};