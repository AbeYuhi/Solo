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

	sceneChange_ = SceneChange::GetInstance();

	//カメラ
	titleCamera_ = std::make_unique<TitleCamera>();
	titleCamera_->Initialize();

	leftMouseClickTexture_ = TextureManager::Load("leftClick.png");
	titleNameTexture_ = TextureManager::Load("titleName.png");
	//speedLinesTexture_ = TextureManager::Load("speedLines.png");

	leftMosueClickSprite_ = Sprite::Create();
	leftMouseClickInfo_.Initialize(leftMouseClickTexture_, {1280, 720}, {0.0f, 0.0f});
	titleNameSprite_ = Sprite::Create();
	titleNameInfo_.Initialize(titleNameTexture_, { 360, 120 }, { 0.5f, 0.5f });
	titleNameInfo_.worldTransform_.data_.translate_ = { 640, 150, 0 };

	speedLinesSprite_ = Sprite::Create();
	speedLinesInfo_.Initialize(speedLinesTexture_, { 1280, 720 }, { 0.0f, 0.0f });
	speedLinesInfo_.materialInfo_.material_->color.w = 0.2f;

	levelScene_.Initialize("titleTest.json");
	titleCamera_->SetWorldTransrom(levelScene_.GetCameraData().CameraInfo);

	time_ = 0.0f;
	change_ = false;
}

void TitleScene::Update() {
	titleCamera_->Update();
	mainCamera_->Update(titleCamera_->GetWorldTransrom(), titleCamera_->GetWorldMatrix(), titleCamera_->GetProjectionMatrix());
	spriteCamera_->Update();

	time_ += 1.0f / 60.0f;
	if (time_ >= 1.0f) {
		time_ -= 1.0f;
	}

	if (titleCamera_->GetWorldTransrom().translate_.z >= 70) {
		titleCamera_->SetPos({ 0.0f, 7.5f, -20.0f });
	}

	if (InputManager::GetInstance()->IsMouseTrigger(0)) {
		change_ = true;
	}

	if (change_) {
		changeTime_ += 1.0f / 60.0f;
		titleCamera_->transform_.translate_.z += (6.0f * (changeTime_ + 2.0f)) * (1.0f / 60.0f);

		if (changeTime_ >= 5.0f) {
			sceneNo_ = INGAME;
			sceneChange_->StartSceneChange();
		}
	}
	else {
		changeTime_ = 0.0f;
		titleCamera_->transform_.translate_.z += 6.0f * (1.0f / 60.0f);
	}

	levelScene_.Update();
}

void TitleScene::Draw() {


	titleNameSprite_->Draw(titleNameInfo_);

	if(change_){
		//speedLinesSprite_->Draw(speedLinesInfo_);
	}
	else {
		if (time_ <= 0.5f) {
			leftMosueClickSprite_->Draw(leftMouseClickInfo_);
		}
	}

	levelScene_.Draw();
}