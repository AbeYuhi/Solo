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
	titleNameTexture_ = TextureManager::Load("titleName.png");

	leftMosueClickSprite_ = Sprite::Create();
	leftMouseClickInfo_.Initialize(leftMouseClickTexture_, {1280, 720}, {0.0f, 0.0f});
	titleNameSprite_ = Sprite::Create();
	titleNameInfo_.Initialize(titleNameTexture_, { 360, 120 }, { 0.5f, 0.5f });
	titleNameInfo_.worldTransform_.data_.translate_ = { 640, 150, 0 };

	levelScene_.Initialize("titleTest.json");

	time_ = 0.0f;
}

void TitleScene::Update() {
	titleCamera_->Update();
	spriteCamera_->Update();

	time_ += 1.0f / 60.0f;
	if (time_ >= 1.0f) {
		time_ -= 1.0f;
	}

	if (levelScene_.CameraPos().z >= 70) {
		levelScene_.SetCameraPos({0.0f, 7.5f, -20.0f});
	}

	if (InputManager::GetInstance()->IsMouseTrigger(0)) {
		sceneNo_ = INGAME;
	}

	levelScene_.Update();
}

void TitleScene::Draw() {

	if (time_ <= 0.5f) {
		leftMosueClickSprite_->Draw(leftMouseClickInfo_);
	}

	titleNameSprite_->Draw(titleNameInfo_);

	levelScene_.Draw();
}