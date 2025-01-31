#include "GameOverScene.h"

/// <summary>
/// GameOverScene.cpp
/// ゲームオーバーシーン
/// </summary>

GameOverScene::GameOverScene() {}

GameOverScene::~GameOverScene() {}

void GameOverScene::Finalize() {

}

void GameOverScene::Initialize() {
	sceneNo_ = GAMEOVER;

	mainCamera_ = MainCamera::GetInstance();
	spriteCamera_ = SpriteCamera::GetInstance();
	spriteCamera_->Initialize();

	leftMouseClickTexture_ = MyEngine::TextureManager::Load("leftClick.png");

	leftMosueClickSprite_ = MyEngine::Sprite::Create();
	leftMouseClickInfo_.Initialize(leftMouseClickTexture_, { 1280, 720 }, { 0.0f, 0.0f });

	gameOverTexture_ = MyEngine::TextureManager::Load("gameOver.png");

	gameOverSprite_ = MyEngine::Sprite::Create();
	gameOverInfo_.Initialize(gameOverTexture_, { 1280, 720 }, { 0.0f, 0.0f });
}

void GameOverScene::Update() {
	spriteCamera_->Update();

	if (MyEngine::InputManager::GetInstance()->IsMouseTrigger(0)) {
		sceneNo_ = TITLE;
	}
}

void GameOverScene::Draw() {


	leftMosueClickSprite_->Draw(leftMouseClickInfo_);
	gameOverSprite_->Draw(gameOverInfo_);
}