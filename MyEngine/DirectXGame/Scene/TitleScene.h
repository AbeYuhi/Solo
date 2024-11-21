#pragma once
#include "IScene.h"
#include "DirectXGame/GameObject/Camera/TitleCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"
#include "Manager/PostEffectManager.h"
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

	uint32_t leftMouseClickTexture_;
	std::shared_ptr<MyEngine::Sprite> leftMosueClickSprite_;
	MyEngine::SpriteItem leftMouseClickInfo_;

	uint32_t titleNameTexture_;
	std::shared_ptr<MyEngine::Sprite> titleNameSprite_;
	MyEngine::SpriteItem titleNameInfo_;

	uint32_t backGroundTexture_;
	std::shared_ptr<MyEngine::Sprite> backGroundSprite_;
	MyEngine::SpriteItem backGroundInfo_;

	uint32_t scoreTexture_;
	uint32_t clearTexture_;
	std::shared_ptr<MyEngine::Sprite> scoreSprite_;
	MyEngine::SpriteItem scoreInfo_;

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