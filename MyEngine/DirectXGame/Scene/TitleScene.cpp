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

	postEffectManager_ = MyEngine::PostEffectManager::GetInstance();

	sceneChange_ = SceneChange::GetInstance();

	//カメラ
	titleCamera_ = std::make_unique<TitleCamera>();
	titleCamera_->Initialize();

	leftMouseClickTexture_ = MyEngine::TextureManager::Load("leftClick.png");
	titleNameTexture_ = MyEngine::TextureManager::Load("titleName.png");
	scoreTexture_ = MyEngine::TextureManager::Load("score.png");
	clearTexture_ = MyEngine::TextureManager::Load("gameClear.png");

	leftMosueClickSprite_ = MyEngine::Sprite::Create();
	leftMouseClickInfo_.Initialize(leftMouseClickTexture_, {1280, 720}, {0.0f, 0.0f});
	titleNameSprite_ = MyEngine::Sprite::Create();
	titleNameInfo_.Initialize(titleNameTexture_, { 360, 120 }, { 0.5f, 0.5f });
	titleNameInfo_.worldTransform_.data_.translate_ = { 640, 150, 0 };

	scoreSprite_ = MyEngine::Sprite::Create();
	scoreInfo_.Initialize(scoreTexture_, { 600, 600 }, { 0.5f, 0.5f });
	scoreInfo_.worldTransform_.data_.translate_ = {640, 360, 0};
	scoreInfo_.worldTransform_.data_.scale_ = { 0.0f, 0.0f, 1.0f };

	backGroundTexture_ = MyEngine::TextureManager::Load("backGround.png");
	backGroundSprite_ = MyEngine::Sprite::Create();
	backGroundInfo_.Initialize(backGroundTexture_, { 1280, 720 }, { 0.0f, 0.0f });

	levelScene_.Initialize("titleTest.json");
	titleCamera_->SetWorldTransrom(levelScene_.GetCameraData().CameraInfo);

	if (preSceneNo_ == INGAME) {
		isPreviousSceneInGame_ = true;
		isResult_ = true;
		isScoreImageScale_ = true;
		scoreInfo_.spriteData_.textureHandle_ = scoreTexture_;
	}
	else if (preSceneNo_ == GAMECLEAR) {
		isPreviousSceneInGame_ = true;
		isResult_ = true;
		isScoreImageScale_ = true;
		scoreInfo_.spriteData_.textureHandle_ = clearTexture_;
	}
	else {
		isPreviousSceneInGame_ = false;
		isResult_ = false;
	}

	time_ = 0.0f;
	change_ = false;
	postEffectManager_->SetPostEffect(kNone);
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
		if (MyEngine::InputManager::GetInstance()->IsMouseTrigger(0)) {
			isResult_ = false;
			isScoreImageScale_ = true;
		}
	}
	else {
		if (MyEngine::InputManager::GetInstance()->IsMouseTrigger(0) && !change_) {
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
	MyEngine::DirectXCommon::GetInstance()->ClearDepthStencilBuffer();

	if(!change_) {
		if (!isResult_) {
			if (time_ <= 0.5f) {
				leftMosueClickSprite_->Draw(leftMouseClickInfo_);
			}
		}
	}
	
	if (scoreInfo_.worldTransform_.data_.scale_.x > 0) {
		scoreSprite_->Draw(scoreInfo_);
	}
	else {
		titleNameSprite_->Draw(titleNameInfo_);
	}

	levelScene_.Draw();
}