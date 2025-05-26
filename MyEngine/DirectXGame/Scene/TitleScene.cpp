#include "TitleScene.h"

/// <summary>
/// TitleScene.cpp
/// タイトルシーン
/// </summary>

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Finalize() {
	MyEngine::AudioManager::GetInstance()->Finalize();
	levelScene_.reset();
	levelScene1_.reset();
	levelScene2_.reset();
}

void TitleScene::Initialize() {
	sceneNo_ = TITLE;

	//インスタンスの取得
	mainCamera_ = MainCamera::GetInstance();
	spriteCamera_ = SpriteCamera::GetInstance();
	spriteCamera_->Initialize();

	postEffectManager_ = MyEngine::PostEffectManager::GetInstance();

	collisionManager_ = MyEngine::CollisionManager::GetInstance();
	collisionManager_->ClearColliders();
	collisionManager_->Update();

	MyEngine::AudioManager::GetInstance()->Initialize();

	sceneChange_ = SceneChange::GetInstance();

	//カメラ
	titleCamera_ = std::make_unique<TitleCamera>();
	titleCamera_->Initialize();

	//平行光源
	lightObj_ = MyEngine::LightObjectManager::GetInstance();
	lightObj_->InitData();

	//スプライトの初期化処理
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
	scoreInfo_.spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
	scoreInfo_.spriteItem->worldTransform_.data_.scale_ = { 0.0f, 0.0f, 1.0f };
	scoreInfo_.spriteItem->materialInfo_.material_->color.w = 1.0f;

	scoreTexture_ = MyEngine::TextureManager::Load("score.png");

	//ステージの読み込み
	levelScene_ = std::make_unique<LevelScene>();
	levelScene1_ = std::make_unique<LevelScene>();
	levelScene2_ = std::make_unique<LevelScene>();
	levelScene_->Initialize("title.json");
	levelScene1_->Initialize("title.json", 300);
	levelScene2_->Initialize("title.json", 600);
	titleCamera_->SetWorldTransrom(levelScene_->GetCameraData().CameraInfo);

	//前回シーンがゲームだったらスコアを表示するように
	if (preSceneNo_ == INGAME) {
		isPreviousSceneInGame_ = true;
		isResult_ = true;
		isScoreImageScale_ = true;
		resultScore_ = sceneChange_->GetScore();
		scoreInfo_.spriteItem->spriteData_.textureHandle_ = scoreTexture_;
	}
	else {
		isPreviousSceneInGame_ = false;
		isResult_ = false;
	}

	time_ = 0.0f;
	change_ = false;
	postEffectManager_->SetPostEffect(kNone);

	//マウスカーソルの制限解除
	MyEngine::WinApp::GetInstance()->UnlockCursor();

	//サウンドのロード
	bgmIndex_ = MyEngine::AudioManager::GetInstance()->SoundLoadWave("music.wav");

	//スコアテクスチャのロード
	numberSpriteTextures_[0] = MyEngine::TextureManager::Load("numberTexture/scoreTexture/0.png");
	numberSpriteTextures_[1] = MyEngine::TextureManager::Load("numberTexture/scoreTexture/1.png");
	numberSpriteTextures_[2] = MyEngine::TextureManager::Load("numberTexture/scoreTexture/2.png");
	numberSpriteTextures_[3] = MyEngine::TextureManager::Load("numberTexture/scoreTexture/3.png");
	numberSpriteTextures_[4] = MyEngine::TextureManager::Load("numberTexture/scoreTexture/4.png");
	numberSpriteTextures_[5] = MyEngine::TextureManager::Load("numberTexture/scoreTexture/5.png");
	numberSpriteTextures_[6] = MyEngine::TextureManager::Load("numberTexture/scoreTexture/6.png");
	numberSpriteTextures_[7] = MyEngine::TextureManager::Load("numberTexture/scoreTexture/7.png");
	numberSpriteTextures_[8] = MyEngine::TextureManager::Load("numberTexture/scoreTexture/8.png");
	numberSpriteTextures_[9] = MyEngine::TextureManager::Load("numberTexture/scoreTexture/9.png");

	for (int i = 0; i < 7; i++) {
		scoreInfos_[i].Initialize();
	}
	
	// 桁数の計算
	if (resultScore_ == 0) {
		digitCount_ = 1;
	}
	else {
		// std::to_stringを使用して整数を文字列に変換し、その長さで桁数を取得
		digitCount_ = static_cast<int>(std::to_string(resultScore_).length());
	}

	// 桁数に応じたスプライトの基本設定を決定
	float spriteWidth = 0.0f;
	float spriteHeight = 0.0f;
	float baseY = 360.0f; // 基本となるY座標
	float spacing = 0.0f; // 数字間のスペース

	if (digitCount_ == 1) {
		spriteWidth = 240.0f;
		spriteHeight = 400.0f;
		baseY = 400.0f;
	}
	else if (digitCount_ == 2) {
		spriteWidth = 240.0f;
		spriteHeight = 400.0f;
		baseY = 400.0f;
		spacing = 240.0f;
	}
	else if (digitCount_ == 3) {
		spriteWidth = 180.0f;
		spriteHeight = 300.0f;
		baseY = 400.0f;
		spacing = 180.0f;
	}
	else if (digitCount_ <= 5) {
		spriteWidth = 120.0f;
		spriteHeight = 200.0f;
		spacing = 120.0f;
	}
	else if (digitCount_ == 6) {
		spriteWidth = 96.0f;
		spriteHeight = 160.0f;
		spacing = 96.0f;
	}
	else if (digitCount_ == 7) {
		spriteWidth = 84.0f;
		spriteHeight = 140.0f;
		spacing = 84.0f;
	}

	// 全体の横幅を計算
	float totalWidth = (digitCount_ - 1) * spacing;
	// 開始X座標を計算 (画面中央を基準)
	float startX = 640.0f - totalWidth / 2.0f;

	int tempScore = abs(resultScore_);

	// 各桁のスプライトを設定
	for (int i = 0; i < digitCount_; ++i) {
		// i番目の桁の数字を取得
		int digit = 0;
		// 右の桁から取得するため、10^(digitCount - 1 - i) で割る
		digit = (tempScore / static_cast<int>(pow(10, digitCount_ - 1 - i))) % 10;
		// スプライト情報を設定
		auto& scoreInfo = scoreInfos_[i];
		scoreInfo.spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[digit];
		scoreInfo.spriteItem->spriteData_.size_ = { spriteWidth, spriteHeight };
		scoreInfo.spriteItem->worldTransform_.data_.translate_ = { startX + i * spacing, baseY, 0 };
		scoreInfo.spriteItem->worldTransform_.data_.scale_ = { 0.0f, 0.0f, 1.0f }; // 初期スケールは0
	}
}

void TitleScene::Update() {
	preSceneNo_ = TITLE;
	titleCamera_->Update();
	mainCamera_->Update(titleCamera_->GetWorldTransrom(), titleCamera_->GetWorldMatrix(), titleCamera_->GetProjectionMatrix());
	spriteCamera_->Update();
	//ライトの更新
	lightObj_->Update();

	time_ += 1.0f / 60.0f;
	if (time_ >= 1.0f) {
		time_ -= 1.0f;
	}

	//bgmを流す
	if (!MyEngine::AudioManager::GetInstance()->IsSoundPlaying(bgmIndex_)) {
		MyEngine::AudioManager::GetInstance()->SoundPlayWave(bgmIndex_, kBGMVolume_, true);
	}

	//一定距離すすんだらカメラを戻す
	if (titleCamera_->GetWorldTransrom().translate_.z >= 300) {
		titleCamera_->SetPos({ titleCamera_->GetWorldTransrom().translate_.x,
			titleCamera_->GetWorldTransrom().translate_.y,
			titleCamera_->GetWorldTransrom().translate_.z - 300 });
	}

	//インゲームに移行するための処理
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

	//ゲーム後の結果表示シーン
	if (isScoreImageScale_) {

		if (isResult_) {
			scoreInfo_.spriteItem->worldTransform_.data_.scale_.x += (1.0f / 60.0f) * 3.0f;
			scoreInfo_.spriteItem->worldTransform_.data_.scale_.y += (1.0f / 60.0f) * 3.0f;
			for (int i = 0; i < digitCount_; i++) {
				scoreInfos_[i].spriteItem->worldTransform_.data_.scale_.x += (1.0f / 60.0f) * 3.0f;
				scoreInfos_[i].spriteItem->worldTransform_.data_.scale_.y += (1.0f / 60.0f) * 3.0f;
			}

			if (scoreInfo_.spriteItem->worldTransform_.data_.scale_.x >= 1.0f) {
				scoreInfo_.spriteItem->worldTransform_.data_.scale_.x = 1;
				scoreInfo_.spriteItem->worldTransform_.data_.scale_.y = 1;
				for (int i = 0; i < digitCount_; i++) {
					scoreInfos_[i].spriteItem->worldTransform_.data_.scale_.x = 1;
					scoreInfos_[i].spriteItem->worldTransform_.data_.scale_.y = 1;
				}
				isScoreImageScale_ = false;
			}
		}
		else {
			scoreInfo_.spriteItem->worldTransform_.data_.scale_.x -= (1.0f / 60.0f) * 5.0f;
			scoreInfo_.spriteItem->worldTransform_.data_.scale_.y -= (1.0f / 60.0f) * 5.0f;
			for (int i = 0; i < digitCount_; i++) {
				scoreInfos_[i].spriteItem->worldTransform_.data_.scale_.x -= (1.0f / 60.0f) * 3.0f;
				scoreInfos_[i].spriteItem->worldTransform_.data_.scale_.y -= (1.0f / 60.0f) * 3.0f;
			}

			if (scoreInfo_.spriteItem->worldTransform_.data_.scale_.x <= 0.0f) {
				scoreInfo_.spriteItem->worldTransform_.data_.scale_.x = 0;
				scoreInfo_.spriteItem->worldTransform_.data_.scale_.y = 0;
				for (int i = 0; i < digitCount_; i++) {
					scoreInfos_[i].spriteItem->worldTransform_.data_.scale_.x = 0;
					scoreInfos_[i].spriteItem->worldTransform_.data_.scale_.y = 0;
				}
				isScoreImageScale_ = false;
			}
		}
	}

	//シーンチェンジ時の演出
	if (change_) {
		changeTime_ += 1.0f / 60.0f;
		titleCamera_->transform_.translate_.z += (6.0f * (changeTime_ + 2.0f)) * (1.0f / 60.0f);
		postEffectManager_->GetRadialBlurInfo()->blurWidth += 0.00005f;
		if (changeTime_ >= 3.0f) {
			sceneNo_ = INGAME;
			sceneChange_->StartSceneChange();
			postEffectManager_->GetRadialBlurInfo()->blurWidth = 0.015f;
		}
	}
	else {
		changeTime_ = 0.0f;
		titleCamera_->transform_.translate_.z += 6.0f * (1.0f / 60.0f);
	}

	//ステージのアップデート
	levelScene_->Update();
	levelScene1_->Update();
	levelScene2_->Update();
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

		for (int i = 0; i < digitCount_; i++) {
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[i]);
		}

	}
	else {
		DrawManager::GetInstance()->PushBackForegroundSprite(&titleNameInfo_);
	}
	levelScene_->Draw();
	levelScene1_->Draw();
	levelScene2_->Draw();

}