#include "TitleScene.h"

/// <summary>
/// TitleScene.cpp
/// タイトルシーン
/// </summary>

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

	postEffectManager_ = PostEffectManager::GetInstance();

	sceneChange_ = SceneChange::GetInstance();

	//カメラ
	titleCamera_ = std::make_unique<TitleCamera>();
	titleCamera_->Initialize();

	leftMouseClickTexture_ = TextureManager::Load("leftClick.png");
	titleNameTexture_ = TextureManager::Load("titleName.png");
	scoreTexture_ = TextureManager::Load("score.png");
	//speedLinesTexture_ = TextureManager::Load("speedLines.png");

	leftMosueClickSprite_ = Sprite::Create();
	leftMouseClickInfo_.Initialize(leftMouseClickTexture_, {1280, 720}, {0.0f, 0.0f});
	titleNameSprite_ = Sprite::Create();
	titleNameInfo_.Initialize(titleNameTexture_, { 360, 120 }, { 0.5f, 0.5f });
	titleNameInfo_.worldTransform_.data_.translate_ = { 640, 150, 0 };

	speedLinesSprite_ = Sprite::Create();
	speedLinesInfo_.Initialize(speedLinesTexture_, { 1280, 720 }, { 0.0f, 0.0f });
	speedLinesInfo_.materialInfo_.material_->color.w = 0.2f;

	scoreSprite_ = Sprite::Create();
	scoreInfo_.Initialize(scoreTexture_, { 600, 600 }, { 0.5f, 0.5f });
	scoreInfo_.worldTransform_.data_.translate_ = {640, 360, 0};
	scoreInfo_.worldTransform_.data_.scale_ = { 0.0f, 0.0f, 1.0f };

	backGroundTexture_ = TextureManager::Load("backGround.png");
	backGroundSprite_ = Sprite::Create();
	backGroundInfo_.Initialize(backGroundTexture_, { 1280, 720 }, { 0.0f, 0.0f });

	levelScene_.Initialize("titleTest.json");
	titleCamera_->SetWorldTransrom(levelScene_.GetCameraData().CameraInfo);

	if (preSceneNo_ == INGAME) {
		isPreviousSceneInGame_ = true;
		isResult_ = true;
		isScoreImageScale_ = true;
	}
	else {
		isPreviousSceneInGame_ = false;
		isResult_ = false;
	}

	time_ = 0.0f;
	change_ = false;
	postEffectManager_->SetPostEffect(kVignetteBlur);
}

void TitleScene::Update() {
	preSceneNo_ = TITLE;
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

	if (isResult_) {
		if (InputManager::GetInstance()->IsMouseTrigger(0)) {
			isResult_ = false;
			isScoreImageScale_ = true;
		}
	}
	else {
		if (InputManager::GetInstance()->IsMouseTrigger(0) && !change_) {
			change_ = true;
			postEffectManager_->SetPostEffect(kRadialBlur);
			postEffectManager_->GetRadialBlurInfo()->blurWidth = 0.0f;
		}
	}

	if (isScoreImageScale_) {

		if (isResult_) {
			scoreInfo_.worldTransform_.data_.scale_.x += (1.0f / 60.0f) * 3.0f;
			scoreInfo_.worldTransform_.data_.scale_.y += (1.0f / 60.0f) * 3.0f;

			if (scoreInfo_.worldTransform_.data_.scale_.x >= 1.0f) {
				scoreInfo_.worldTransform_.data_.scale_.x = 1;
				scoreInfo_.worldTransform_.data_.scale_.y = 1;
				isScoreImageScale_ = false;
			}
		}
		else {
			scoreInfo_.worldTransform_.data_.scale_.x -= (1.0f / 60.0f) * 5.0f;
			scoreInfo_.worldTransform_.data_.scale_.y -= (1.0f / 60.0f) * 5.0f;

			if (scoreInfo_.worldTransform_.data_.scale_.x <= 0.0f) {
				scoreInfo_.worldTransform_.data_.scale_.x = 0;
				scoreInfo_.worldTransform_.data_.scale_.y = 0;
				isScoreImageScale_ = false;
			}
		}
	}

	if (change_) {
		changeTime_ += 1.0f / 60.0f;
		titleCamera_->transform_.translate_.z += (6.0f * (changeTime_ + 2.0f)) * (1.0f / 60.0f);
		postEffectManager_->GetRadialBlurInfo()->blurWidth += 0.00005f;
		if (changeTime_ >= 5.0f) {
			sceneNo_ = INGAME;
			sceneChange_->StartSceneChange();
			postEffectManager_->GetRadialBlurInfo()->blurWidth = 0.015f;
		}
	}
	else {
		changeTime_ = 0.0f;
		titleCamera_->transform_.translate_.z += 6.0f * (1.0f / 60.0f);
	}

	levelScene_.Update();
}

void TitleScene::Draw() {
	backGroundSprite_->Draw(backGroundInfo_);

	///背景スプライト描画終了
	//深度バッファのクリア
	DirectXCommon::GetInstance()->ClearDepthStencilBuffer();

	titleNameSprite_->Draw(titleNameInfo_);

	if (change_) {
		//speedLinesSprite_->Draw(speedLinesInfo_);
	}
	else {
		if (!isResult_) {
			if (time_ <= 0.5f) {
				leftMosueClickSprite_->Draw(leftMouseClickInfo_);
			}
		}
	}
	
	if (scoreInfo_.worldTransform_.data_.scale_.x > 0) {
		scoreSprite_->Draw(scoreInfo_);
	}

	levelScene_.Draw();
}