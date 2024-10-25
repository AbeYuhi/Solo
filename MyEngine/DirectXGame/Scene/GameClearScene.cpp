#include "GameClearScene.h"

/// <summary>
/// GameClear.cpp
/// ゲームクリアシーン
/// </summary>

GameClearScene::GameClearScene() {}
GameClearScene::~GameClearScene() {}

void GameClearScene::Finalize() {
	
}

void GameClearScene::Initialize() {
	sceneNo_ = GAMECLEAR;

	mainCamera_ = MainCamera::GetInstance();
	spriteCamera_ = SpriteCamera::GetInstance();
	spriteCamera_->Initialize();

	leftMouseClickTexture_ = TextureManager::Load("leftClick.png");

	leftMosueClickSprite_ = Sprite::Create();
	leftMouseClickInfo_.Initialize(leftMouseClickTexture_, { 1280, 720 }, { 0.0f, 0.0f });

	gameOverTexture_ = TextureManager::Load("gameClear.png");

	gameOverSprite_ = Sprite::Create();
	gameOverInfo_.Initialize(gameOverTexture_, { 1280, 720 }, { 0.0f, 0.0f });
}

void GameClearScene::Update() {
	spriteCamera_->Update();

	if (InputManager::GetInstance()->IsMouseTrigger(0)) {
		sceneNo_ = TITLE;
	}
}

void GameClearScene::Draw() {


	leftMosueClickSprite_->Draw(leftMouseClickInfo_);
	gameOverSprite_->Draw(gameOverInfo_);
}