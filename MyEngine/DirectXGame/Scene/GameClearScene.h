#pragma once
#include "IScene.h"
#include "Object/Sprite.h"
#include "DirectXGame/GameObject/Camera/GameClearCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"

class GameClearScene : public IScene
{
public:
	GameClearScene();
	~GameClearScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;
private:
	MainCamera* mainCamera_;
	SpriteCamera* spriteCamera_;

	uint32_t leftMouseClickTexture_;
	std::shared_ptr<Sprite> leftMosueClickSprite_;
	SpriteItem leftMouseClickInfo_;

	uint32_t gameOverTexture_;
	std::shared_ptr<Sprite> gameOverSprite_;
	SpriteItem gameOverInfo_;
};