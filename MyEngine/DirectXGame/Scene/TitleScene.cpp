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

	collisionManager_ = MyEngine::CollisionManager::GetInstance();
	collisionManager_->ClearColliders();
	collisionManager_->Update();

	sceneChange_ = SceneChange::GetInstance();

	//カメラ
	titleCamera_ = std::make_unique<TitleCamera>();
	titleCamera_->Initialize();

	leftMouseClickInfo_.Initialize();
	leftMouseClickInfo_.spriteItem->spriteData_.textureHandle_ = MyEngine::TextureManager::Load("leftClick.png");
	leftMouseClickInfo_.spriteItem->spriteData_.size_ = { 1280, 720 };
	leftMouseClickInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.0f, 0.0f };
	leftMouseClickInfo_.spriteItem->materialInfo_.material_->color.w = 1.0f;

	titleNameInfo_.Initialize();
	titleNameInfo_.spriteItem->spriteData_.textureHandle_ = MyEngine::TextureManager::Load("titleName.png");
	titleNameInfo_.spriteItem->spriteData_.size_ = { 360, 120 };
	titleNameInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.5f, 0.5f };
	titleNameInfo_.spriteItem->worldTransform_.data_.translate_ = { 640, 150, 0 };
	titleNameInfo_.spriteItem->materialInfo_.material_->color.w = 1.0f;

	backGroundInfo_.Initialize();
	backGroundInfo_.spriteItem->spriteData_.textureHandle_ = MyEngine::TextureManager::Load("backGround.png");
	backGroundInfo_.spriteItem->spriteData_.size_ = { 1280, 720 };
	backGroundInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.0f, 0.0f };
	backGroundInfo_.spriteItem->materialInfo_.material_->color.w = 1.0f;

	scoreInfo_.Initialize();
	scoreInfo_.spriteItem->spriteData_.textureHandle_ = MyEngine::TextureManager::Load("score.png");
	scoreInfo_.spriteItem->spriteData_.size_ = { 600, 600 };
	scoreInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.5f, 0.5f };
	scoreInfo_.spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
	scoreInfo_.spriteItem->worldTransform_.data_.scale_ = { 0.0f, 0.0f, 1.0f };
	scoreInfo_.spriteItem->materialInfo_.material_->color.w = 1.0f;

	scoreTexture_ = MyEngine::TextureManager::Load("score.png");
	clearTexture_ = MyEngine::TextureManager::Load("gameClear.png");

	levelScene_.Initialize("test.json");
	titleCamera_->SetWorldTransrom(levelScene_.GetCameraData().CameraInfo);

	if (preSceneNo_ == INGAME) {
		isPreviousSceneInGame_ = true;
		isResult_ = true;
		isScoreImageScale_ = true;
		scoreInfo_.spriteItem->spriteData_.textureHandle_ = scoreTexture_;
	}
	else if (preSceneNo_ == GAMECLEAR) {
		isPreviousSceneInGame_ = true;
		isResult_ = true;
		isScoreImageScale_ = true;
		scoreInfo_.spriteItem->spriteData_.textureHandle_ = clearTexture_;
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
		titleCamera_->SetPos({ 0.0f, 5.0f, -20.0f });
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
			scoreInfo_.spriteItem->worldTransform_.data_.scale_.x += (1.0f / 60.0f) * 3.0f;
			scoreInfo_.spriteItem->worldTransform_.data_.scale_.y += (1.0f / 60.0f) * 3.0f;

			if (scoreInfo_.spriteItem->worldTransform_.data_.scale_.x >= 1.0f) {
				scoreInfo_.spriteItem->worldTransform_.data_.scale_.x = 1;
				scoreInfo_.spriteItem->worldTransform_.data_.scale_.y = 1;
				isScoreImageScale_ = false;
			}
		}
		else {
			scoreInfo_.spriteItem->worldTransform_.data_.scale_.x -= (1.0f / 60.0f) * 5.0f;
			scoreInfo_.spriteItem->worldTransform_.data_.scale_.y -= (1.0f / 60.0f) * 5.0f;

			if (scoreInfo_.spriteItem->worldTransform_.data_.scale_.x <= 0.0f) {
				scoreInfo_.spriteItem->worldTransform_.data_.scale_.x = 0;
				scoreInfo_.spriteItem->worldTransform_.data_.scale_.y = 0;
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
	DrawManager::GetInstance()->PushBackBackgroundSprite(&backGroundInfo_);

	if(!change_) {
		if (!isResult_) {
			if (time_ <= 0.5f) {
				DrawManager::GetInstance()->PushBackForegroundSprite(&leftMouseClickInfo_);
			}
		}
	}
	
	if (scoreInfo_.spriteItem->worldTransform_.data_.scale_.x > 0) {
		DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfo_);
	}
	else {
		DrawManager::GetInstance()->PushBackForegroundSprite(&titleNameInfo_);
	}
	levelScene_.Draw();

}