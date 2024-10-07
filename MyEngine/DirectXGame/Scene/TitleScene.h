#pragma once
#include "IScene.h"
#include "DirectXGame/GameObject/Camera/TitleCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"
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

	float time_;

	LevelScene levelScene_;
};