#pragma once
#include "IScene.h"
#include "DirectXGame/GameObject/Camera/TitleCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"
#include "Scene/SceneChange/SceneChange.h"
#include "Data/SpriteItem.h"
#include "Object/Model.h"
#include "Object/Sprite.h"
#include "LoadFile/LevelScene.h"

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

	std::unique_ptr<TitleCamera> titleCamera_;

	CollisionManager* collisionManager_;

	uint32_t leftMouseClickTexture_;
	std::shared_ptr<Sprite> leftMosueClickSprite_;
	SpriteItem leftMouseClickInfo_;

	uint32_t titleNameTexture_;
	std::shared_ptr<Sprite> titleNameSprite_;
	SpriteItem titleNameInfo_;

	uint32_t speedLinesTexture_;
	std::shared_ptr<Sprite> speedLinesSprite_;
	SpriteItem speedLinesInfo_;

	SceneChange* sceneChange_;

	float time_;
	LevelScene levelScene_;

	float changeTime_;
	bool change_;
};