#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Finalize() {

}

void TitleScene::Initialize() {
	sceneNo_ = TITLE;

	//
	mainCamera_ = MainCamera::GetInstance();
	spriteCamera_ = SpriteCamera::GetInstance();
	spriteCamera_->Initialize();

	//カメラ
	titleCamera_ = std::make_unique<TitleCamera>();
	titleCamera_->Initialize();

	leftMouseClickTexture_ = TextureManager::Load("leftClick.png");

	leftMosueClickSprite_ = Sprite::Create();
	leftMouseClickInfo_.Initialize(leftMouseClickTexture_, {1280, 720}, {0.0f, 0.0f});
}

void TitleScene::Update() {
	titleCamera_->Update();
	spriteCamera_->Update();

	if (InputManager::GetInstance()->IsMouseTrigger(0)) {
		sceneNo_ = INGAME;
	}
}

void TitleScene::Draw() {

	leftMosueClickSprite_->Draw(leftMouseClickInfo_);
}