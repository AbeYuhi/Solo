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
	scoreInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.5f, 0.5f };
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
	//テスト
	preSceneNo_ = GAMECLEAR;
	if (preSceneNo_ == INGAME || preSceneNo_ == GAMECLEAR) {
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

	//照すt
	resultScore_ = 15;

	time_ = 0.0f;
	change_ = false;
	postEffectManager_->SetPostEffect(kNone);

	//マウスカーソルの制限解除
	MyEngine::WinApp::GetInstance()->UnlockCursor();

	//サウンドのロード
	bgmIndex_ = MyEngine::AudioManager::GetInstance()->SoundLoadWave("music.wav");

	//スコアテクスチャのロード
	numberSpriteTextures_[0] = MyEngine::TextureManager::Load("numberTexture/0.png");
	numberSpriteTextures_[1] = MyEngine::TextureManager::Load("numberTexture/1.png");
	numberSpriteTextures_[2] = MyEngine::TextureManager::Load("numberTexture/2.png");
	numberSpriteTextures_[3] = MyEngine::TextureManager::Load("numberTexture/3.png");
	numberSpriteTextures_[4] = MyEngine::TextureManager::Load("numberTexture/4.png");
	numberSpriteTextures_[5] = MyEngine::TextureManager::Load("numberTexture/5.png");
	numberSpriteTextures_[6] = MyEngine::TextureManager::Load("numberTexture/6.png");
	numberSpriteTextures_[7] = MyEngine::TextureManager::Load("numberTexture/7.png");
	numberSpriteTextures_[8] = MyEngine::TextureManager::Load("numberTexture/8.png");
	numberSpriteTextures_[9] = MyEngine::TextureManager::Load("numberTexture/9.png");

	for (int i = 0; i < 5; i++) {
		scoreInfos_[i].Initialize();
	}

	//桁数の計算
	digitCount_ = 0;
	int number = resultScore_;
	if (number == 0) {
		digitCount_ = 1;
	}
	else {
		// 負の数の場合は正の数に変換
		number = abs(number);
		while (number > 0) {
			//10進数だから10で割る
			number /= 10;
			digitCount_++;
		}
	}

	//桁数に応じた処理
	if (digitCount_ == 1) {
		scoreInfos_[0].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 0)];
		//玉の桁数により表示位置の変更
		scoreInfos_[0].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[0].spriteItem->spriteData_.size_ = {240, 400};
	}
	else if (digitCount_ == 2) {
		scoreInfos_[1].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 1)];
		scoreInfos_[0].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 0)];
		//玉の桁数により表示位置の変更
		scoreInfos_[1].spriteItem->worldTransform_.data_.translate_ = { 520, 360, 0 };
		scoreInfos_[0].spriteItem->worldTransform_.data_.translate_ = { 760, 360, 0 };
		scoreInfos_[1].spriteItem->spriteData_.size_ = { 240, 400 };
		scoreInfos_[0].spriteItem->spriteData_.size_ = { 240, 400 };
	}
	else if (digitCount_ == 3) {
		scoreInfos_[2].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 2)];
		scoreInfos_[1].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 1)];
		scoreInfos_[0].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 0)];
		//玉の桁数により表示位置の変更
		scoreInfos_[2].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[1].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[0].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[2].spriteItem->spriteData_.size_ = {240, 400};
		scoreInfos_[1].spriteItem->spriteData_.size_ = {240, 400};
		scoreInfos_[0].spriteItem->spriteData_.size_ = {240, 400};
	}
	else if (digitCount_ == 4) {
		scoreInfos_[3].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 3)];
		scoreInfos_[2].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 2)];
		scoreInfos_[1].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 1)];
		scoreInfos_[0].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 0)];
		//玉の桁数により表示位置の変更
		scoreInfos_[3].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[2].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[1].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[0].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[3].spriteItem->spriteData_.size_ = {240, 400};
		scoreInfos_[2].spriteItem->spriteData_.size_ = {240, 400};
		scoreInfos_[1].spriteItem->spriteData_.size_ = {240, 400};
		scoreInfos_[0].spriteItem->spriteData_.size_ = {240, 400};
	}
	else if (digitCount_ == 5) {
		scoreInfos_[4].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 4)];
		scoreInfos_[3].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 3)];
		scoreInfos_[2].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 2)];
		scoreInfos_[1].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 1)];
		scoreInfos_[0].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(resultScore_, 0)];
		//玉の桁数により表示位置の変更
		scoreInfos_[4].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[3].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[2].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[1].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[0].spriteItem->worldTransform_.data_.translate_ = { 640, 360, 0 };
		scoreInfos_[4].spriteItem->spriteData_.size_ = {240, 400};
		scoreInfos_[3].spriteItem->spriteData_.size_ = {240, 400};
		scoreInfos_[2].spriteItem->spriteData_.size_ = {240, 400};
		scoreInfos_[1].spriteItem->spriteData_.size_ = {240, 400};
		scoreInfos_[0].spriteItem->spriteData_.size_ = {240, 400};
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

		//残り玉数の描画
		if (digitCount_ == 1) {
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[0]);
		}
		else if (digitCount_ == 2) {
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[1]);
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[0]);
		}														
		else if (digitCount_ == 3) {							
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[2]);
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[1]);
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[0]);
		}
		else if (digitCount_ == 4) {
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[3]);
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[2]);
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[1]);
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[0]);
		}
		else if (digitCount_ == 5) {
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[4]);
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[3]);
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[2]);
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[1]);
			DrawManager::GetInstance()->PushBackForegroundSprite(&scoreInfos_[0]);
		}

	}
	else {
		DrawManager::GetInstance()->PushBackForegroundSprite(&titleNameInfo_);
	}
	levelScene_->Draw();
	levelScene1_->Draw();
	levelScene2_->Draw();

}