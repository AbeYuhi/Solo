#include "InGameScene.h"

/// <summary>
/// InGameScene.cpp
/// インゲームシーン
/// </summary>

InGameScene::InGameScene() {

}

InGameScene::~InGameScene() {

}

void InGameScene::Finalize() {}

void InGameScene::Initialize() {
	sceneNo_ = INGAME;

	//基本機能
	winApp_ = MyEngine::WinApp::GetInstance();
	directXCommon_ = MyEngine::DirectXCommon::GetInstance();
	input_ = MyEngine::InputManager::GetInstance();
	audioManager_ = MyEngine::AudioManager::GetInstance();
	randomManager_ = MyEngine::RandomManager::GetInstance();
	postEffectManager_ = MyEngine::PostEffectManager::GetInstance();
	collisionManager_ = MyEngine::CollisionManager::GetInstance();
	drawManager_ = DrawManager::GetInstance();
	mainCamera_ = MainCamera::GetInstance();
	spriteCamera_ = SpriteCamera::GetInstance();

	//スプライトカメラの初期化
	spriteCamera_->Initialize();

	//平行光源
	lightObj_ = MyEngine::LightObjectManager::GetInstance();
	lightObj_->InitData();

	//影
	shadow_ = std::make_unique<Shadow>();
	shadow_->Initialize();

	//コリジョンマネージャー
	collisionManager_->Initialize();

	//ステージのjsonを読み込む
	std::unique_ptr<LevelScene> level0 = std::make_unique<LevelScene>();
	//level0->Initialize("stage0.json");
	level0->Initialize("debug.json");
	stageSize_ = level0->GetCameraData().stageSize.z;
	levelScenes_.push_back(std::move(level0));
	std::unique_ptr<LevelScene> level1 = std::make_unique<LevelScene>();
	level1->Initialize("stage1.json", stageSize_);
	levelScenes_.push_back(std::move(level1));
	nowStage_ = 0;

	//インゲームカメラ
	gameCamera_ = std::make_unique<InGameCamera>();
	gameCamera_->Initialize();
	gameCamera_->transform_ = levelScenes_[0]->GetCameraData().CameraInfo;

	player_.Initialize(&gameCamera_->transform_);
	//プレイヤー情報をクリスタルに渡す
	const int kStartStageNum = 2;
	for (int i = 0; i < kStartStageNum; i++) {
		for (auto& crystal : levelScenes_[i]->GetCrystals()) {
			crystal.SetComboDestroyCount(player_.GetComboDestroyCount());
			crystal.SetNumberofSlashAttacks(player_.GetNumberofSlashAttacks());
		}
	}

	//チュートリアル素材の読み込み
	ballShotExplanationInfo_.Initialize();
	ballShotExplanationInfo_.spriteItem->spriteData_.textureHandle_ = MyEngine::TextureManager::Load("ballShot_Explanation.png");
	ballShotExplanationInfo_.spriteItem->spriteData_.size_ = { 1280, 720 };
	ballShotExplanationInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.0f, 0.0f };
	ballShotExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 0.0f;

	crystalExplanationInfo_.Initialize();
	crystalExplanationInfo_.spriteItem->spriteData_.textureHandle_ = MyEngine::TextureManager::Load("crystal_Explanation.png");
	crystalExplanationInfo_.spriteItem->spriteData_.size_ = { 1280, 720 };
	crystalExplanationInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.0f, 0.0f };
	crystalExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 0.0f;

	comboExplanationInfo_.Initialize();
	comboExplanationInfo_.spriteItem->spriteData_.textureHandle_ = MyEngine::TextureManager::Load("combo_Explanation.png");
	comboExplanationInfo_.spriteItem->spriteData_.size_ = { 1280, 720 };
	comboExplanationInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.0f, 0.0f };
	comboExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 0.0f;
	isFirst10Combo_ = false;

	ballLostExplanationInfo_.Initialize();
	ballLostExplanationInfo_.spriteItem->spriteData_.textureHandle_ = MyEngine::TextureManager::Load("ballLost_Explanation.png");
	ballLostExplanationInfo_.spriteItem->spriteData_.size_ = { 1280, 720 };
	ballLostExplanationInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.0f, 0.0f };
	ballLostExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 0.0f;
	isFirstLost_ = false;

	gameOverInfo_.Initialize();
	gameOverInfo_.spriteItem->spriteData_.textureHandle_ = MyEngine::TextureManager::Load("gameOver_Explanation.png");
	gameOverInfo_.spriteItem->spriteData_.size_ = { 1280, 720 };
	gameOverInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.0f, 0.0f };
	gameOverInfo_.spriteItem->materialInfo_.material_->color.w = 0.0f;

	backGroundInfo_.Initialize();
	backGroundInfo_.spriteItem->spriteData_.textureHandle_ = MyEngine::TextureManager::Load("backGround.png");
	backGroundInfo_.spriteItem->spriteData_.size_ = { 1280, 720 };
	backGroundInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.0f, 0.0f };
	backGroundInfo_.spriteItem->materialInfo_.material_->color.w = 1.0f;

	//メンバ変数の初期化
	startTimer_ = 0.0f;
	ballShotExplanationTime_ = 9999.9999f;
	crystalExplanationTime_ = 9999.9999f;
	cameraSpeed_ = { 0.0f, 0.0f, 0.0f };
	cameraEasingTimer_ = 0.0f;
	easingTimer_ = 0.0f;

	postEffectManager_->SetPostEffect(kRadialBlur);

	gameOver_ = false;
	gameOverTimer_ = 0.0f;
	gameClear_ = false;
	gameClearTimer_ = 0.0f;

	cameraSpeed_.z = levelScenes_[0]->GetCameraData().cameraSpeed;
	playerSpeed_ = cameraSpeed_;

	//マウスカーソルに制限をつける
	winApp_->LockCursorToWindow();
}

void InGameScene::Update() {
	preSceneNo_ = INGAME;
	//スプライトカメラの更新
	spriteCamera_->Update();
	gameCamera_->Update();
	//ライトの更新
	lightObj_->Update();
	//影の更新
	shadow_->Update(lightObj_->GetDirectionalLightData(0).direction);

	//現在の状況に沿ったカメラの挙動の処理
	if (gameOver_) {	//ゲームオーバー時のカメラ挙動
		gameOverTimer_ += 1.0f / 60.0f;

		gameOverInfo_.spriteItem->materialInfo_.material_->color.w += 0.05f;
		if (gameOverInfo_.spriteItem->materialInfo_.material_->color.w > 1.0f) {
			gameOverInfo_.spriteItem->materialInfo_.material_->color.w = 1;
		}

		if (gameOverTimer_ >= 1.0f && gameOverTimer_ < 3.0f) {
			cameraEasingTimer_ += 1.0f / 120.0f;
			if (cameraEasingTimer_ > 1.0f) {
				cameraEasingTimer_ = 1.0f;
			}
			easingTimer_ = 1 - std::cos((cameraEasingTimer_ * M_PI) / 2);
			cameraSpeed_.z = (1.0f - easingTimer_) * 5.0f + easingTimer_ * 0.0f;
		}
		else if (gameOverTimer_ >= 3.0f) {
			if (gameOverTimer_ <= 3.1f) {
				cameraEasingTimer_ = 0;
			}
			cameraEasingTimer_ += 1.0f / 60.0f;
			if (cameraEasingTimer_ > 1.0f) {
				cameraEasingTimer_ = 1.0f;
			}
			easingTimer_ = 1 - std::cos((cameraEasingTimer_ * M_PI) / 2);
			cameraSpeed_.z = easingTimer_ * -20.0f;

			postEffectManager_->SetPostEffect(kRadialBlur);
			postEffectManager_->GetRadialBlurInfo()->blurWidth += 0.0001f;
			if (postEffectManager_->GetRadialBlurInfo()->blurWidth >= 0.015f) {
				sceneNo_ = TITLE;
				SceneChange::GetInstance()->StartSceneChange();
			}
		}
		else {
			cameraEasingTimer_ = 0.0f;
		}
	}
	else if (gameClear_) {	//ゲームクリア時のカメラ挙動
		if (gameClearTimer_ == 0.0f) {
			postEffectManager_->GetRadialBlurInfo()->blurWidth = 0.0f;
		}
		
		//カメラスピードのイージングする処理
		gameClearTimer_ += 1.0f / 60.0f;
		cameraEasingTimer_ += 1.0f / 120.0f;
		if (cameraEasingTimer_ > 1.0f) {
			cameraEasingTimer_ = 1.0f;
		}

		float c5 = (2.0f * 3.14f) / 4.5f;
		if (cameraEasingTimer_ == 0.0f) {
			easingTimer_ = 0.0f;
		}
		else if (cameraEasingTimer_ == 1.0f) {
			easingTimer_ = 1.0f;
		}
		else if (cameraEasingTimer_ < 0.5f) {
			easingTimer_ = -(std::pow(2.0f, 20.0f * cameraEasingTimer_ - 10.0f) * std::sin((20.0f * cameraEasingTimer_ - 11.125f) * c5)) / 2.0f;
		}
		else {
			easingTimer_ = (std::pow(2.0f, -20.0f * cameraEasingTimer_ + 10.0f) * std::sin((20.0f * cameraEasingTimer_ - 11.125f) * c5)) / 2.0f + 1.0f;
		}

		cameraSpeed_.z = (1.0f - easingTimer_) * 5.0f + easingTimer_ * 30.0f;
		postEffectManager_->SetPostEffect(kRadialBlur);
		postEffectManager_->GetRadialBlurInfo()->blurWidth += 0.0005f;
		if (postEffectManager_->GetRadialBlurInfo()->blurWidth >= 0.05f) {
			preSceneNo_ = GAMECLEAR;
			sceneNo_ = TITLE;
			SceneChange::GetInstance()->StartSceneChange();
		}
	}
	else {	//通常時のカメラ挙動
		gameOverTimer_ = 0.0f;
		if (startTimer_ >= 1.0f && startTimer_ <= 4.0f) {
			cameraEasingTimer_ += kDeltaTime / 2.0f;
			if (cameraEasingTimer_ > 1.0f) {
				cameraEasingTimer_ = 1.0f;
			}

			easingTimer_ = 1 - std::cos((cameraEasingTimer_ * M_PI) / 2);
			cameraSpeed_.z = (1.0f - easingTimer_) * 30.0f + easingTimer_ * playerSpeed_.z;
			postEffectManager_->GetRadialBlurInfo()->blurWidth = (1.0f - easingTimer_) * 0.015f + easingTimer_ * 0.0f;
			if (cameraEasingTimer_ >= 1.0f) {
				cameraSpeed_.z = playerSpeed_.z;
				player_.SetIsShot(true);
				postEffectManager_->SetPostEffect(kNone);
				postEffectManager_->GetRadialBlurInfo()->blurWidth = 0.0f;
			}
		}
	}

	//カメラを移動
	gameCamera_->transform_.translate_ += cameraSpeed_ * kDeltaTime;

	//ステージの一定量まで進んだら次のステージを読み込むように
	if (stageSize_ <= gameCamera_->transform_.translate_.z) {
		levelScenes_[nowStage_]->Finalize();
		nowStage_++;
		if (kStageNum_ < nowStage_) {
			gameClear_ = true;
		}
		else {
			stageSize_ += levelScenes_[nowStage_]->GetCameraData().stageSize.z;
			cameraSpeed_.z = levelScenes_[nowStage_]->GetCameraData().cameraSpeed;
			if (kStageNum_ != nowStage_) {
				//次のステージを読み込む
				std::string nextStageName = "stage" + to_string(nowStage_ + 1) + ".json";
				std::unique_ptr<LevelScene> level = std::make_unique<LevelScene>();
				level->Initialize(nextStageName, stageSize_);
				levelScenes_.push_back(std::move(level));

				//クリスタル
				for (auto& crystal : levelScenes_[nowStage_ + 1]->GetCrystals()) {
					crystal.SetComboDestroyCount(player_.GetComboDestroyCount());
					crystal.SetNumberofSlashAttacks(player_.GetNumberofSlashAttacks());
				}
			}
		}
	}

	//チュートリアルの処理
	//ボールの発射方法
	startTimer_ += kDeltaTime;
	if (startTimer_ >= kBallShotTutorialStartTime && startTimer_ <= kBallShotTutorialStartTime + kTutorialDuration) {
		ballShotExplanationInfo_.spriteItem->materialInfo_.material_->color.w += kColorFadeStep;
		if (ballShotExplanationInfo_.spriteItem->materialInfo_.material_->color.w > 1.0f) {
			ballShotExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 1;
			isBallShotExplanation_ = true;
			ballShotExplanationTime_ = startTimer_;
		}
	}
	if (isBallShotExplanation_ && ballShotExplanationTime_ + kTutorialEndTime <= startTimer_) {
		ballShotExplanationInfo_.spriteItem->materialInfo_.material_->color.w -= kColorFadeStep;
		if (ballShotExplanationInfo_.spriteItem->materialInfo_.material_->color.w < 0.0f) {
			ballShotExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 0;
		}
	}

	//クリスタルについての説明
	if (startTimer_ >= kCrystalTutorialStartTime && startTimer_ <= kCrystalTutorialStartTime + kTutorialDuration) {
		crystalExplanationInfo_.spriteItem->materialInfo_.material_->color.w += kColorFadeStep;
		if (crystalExplanationInfo_.spriteItem->materialInfo_.material_->color.w > 1.0f) {
			crystalExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 1;
			isCrystalExplanation_ = true;
			crystalExplanationTime_ = startTimer_;
		}
	}
	if (isCrystalExplanation_ && crystalExplanationTime_ + kTutorialEndTime <= startTimer_) {
		crystalExplanationInfo_.spriteItem->materialInfo_.material_->color.w -= kColorFadeStep;
		if (crystalExplanationInfo_.spriteItem->materialInfo_.material_->color.w < 0.0f) {
			crystalExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 0;
		}
	}

	//コンボについての説明
	if (!isComboExplanation_ && isFirst10Combo_) {
		comboExplanationInfo_.spriteItem->materialInfo_.material_->color.w += kColorFadeStep;
		if (comboExplanationInfo_.spriteItem->materialInfo_.material_->color.w > 1.0f) {
			comboExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 1;
			isComboExplanation_ = true;
			comboExplanationTime_ = startTimer_;
		}
	}
	if (isComboExplanation_ && comboExplanationTime_ + kTutorialEndTime <= startTimer_) {
		comboExplanationInfo_.spriteItem->materialInfo_.material_->color.w -= kColorFadeStep;
		if (comboExplanationInfo_.spriteItem->materialInfo_.material_->color.w < 0.0f) {
			comboExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 0;
		}
	}

	//ボールロストについての説明
	if (!isballLostExplanation_ && isFirstLost_) {
		ballLostExplanationInfo_.spriteItem->materialInfo_.material_->color.w += kColorFadeStep;
		if (ballLostExplanationInfo_.spriteItem->materialInfo_.material_->color.w > 1.0f) {
			ballLostExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 1;
			isballLostExplanation_ = true;
			ballLostExplanationTime_ = startTimer_;
		}
	}
	if (isballLostExplanation_ && ballLostExplanationTime_ + kTutorialEndTime <= startTimer_) {
		ballLostExplanationInfo_.spriteItem->materialInfo_.material_->color.w -= kColorFadeStep;
		if (ballLostExplanationInfo_.spriteItem->materialInfo_.material_->color.w < 0.0f) {
			ballLostExplanationInfo_.spriteItem->materialInfo_.material_->color.w = 0;
		}
	}

	//プレイヤーの処理
	if (!gameClear_ && !gameOver_) {
		player_.Update();
		if (!isFirst10Combo_) {
			if (*player_.GetComboDestroyCount() >= 10) {
				isFirst10Combo_ = true;
			}
		}

		if (!isFirstLost_) {
			if (player_.IsBallLost()) {
				isFirstLost_ = true;
			}
		}
	}

	//ステージの処理
	for (int index = nowStage_; index < levelScenes_.size(); index++) {
		levelScenes_[index]->Update();
	}

	//プレイヤーの状態に応じてゲームクリアなどに移行
	if (player_.IsGameOver()) {
		gameOver_ = true;
	}
	else {
		gameOver_ = false;
	}

	//コリジョンマネージャーを更新
	collisionManager_->Update();
#ifdef _DEBUG

	ImGui::Begin("RenderItemInfo");
	ImGui::BeginTabBar("RenderItemInfo");
	ImGui::EndTabBar();
	ImGui::End();

	ImGui::Begin("PostEffect");
	const char* postEffects[] = { "None", "Copy", "HSVFilter", "GrayScale", "SepiaScale", "Vignette", "VignetteBlur", "Smoothing", "RadialBlur"};
	int postEffect = postEffectManager_->GetPostEffect();
	ImGui::Combo("postEffect", &postEffect, postEffects, IM_ARRAYSIZE(postEffects));
	postEffectManager_->SetPostEffect(static_cast<PostEffect>(postEffect));

	ImGui::BeginTabBar("PostEffectState");
	if (ImGui::BeginTabItem("HSVFilter")) {
		ImGui::SliderFloat("hue", &postEffectManager_->GetHSVMaterial()->hue, -1.0f, 1.0f);
		ImGui::SliderFloat("saturation", &postEffectManager_->GetHSVMaterial()->saturation, -1.0f, 1.0f);
		ImGui::SliderFloat("value", &postEffectManager_->GetHSVMaterial()->value, -1.0f, 1.0f);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Smoothing")) {
		int32_t kernelSize = postEffectManager_->GetKernelSize();
		ImGui::SliderInt("size", &kernelSize, 1, 5);
		postEffectManager_->SetKernelSize(kernelSize);

		const char* types[] = { "Box", "Gauss" };
		int32_t type = postEffectManager_->GetSmoothingType();
		ImGui::Combo("SmoothingType", &type, types, IM_ARRAYSIZE(types));
		postEffectManager_->SetSmoothingType(type);

		float strength = postEffectManager_->GetBlurStrength();
		ImGui::SliderFloat("Strength", &strength, 1.0f, 5.0f);
		postEffectManager_->SetBlurStrength(strength);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("RadialBlur")) {
		ImGui::SliderInt("numSamples", &postEffectManager_->GetRadialBlurInfo()->numSamples, 0, 20);
		ImGui::SliderFloat("blurWidth", &postEffectManager_->GetRadialBlurInfo()->blurWidth, 0.0f, 0.1f);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("VignetteBlur")) {
		ImGui::SliderFloat("intensity", &postEffectManager_->GetVignetteBlurInfo()->intensity, 0.0f, 10.0f);
		ImGui::SliderFloat("blurAmount", &postEffectManager_->GetVignetteBlurInfo()->blurAmount, 0.0f, 5.0f);
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();

	ImGui::End();

#endif // _DEBUG
}

void InGameScene::Draw() {

	//描画処理をmanagerに積む
	drawManager_->PushBackBackgroundSprite(&backGroundInfo_);

	//プレイヤーの描画
	player_.Draw();

	//レベルシーンの描画
	for (int index = nowStage_; index < levelScenes_.size(); index++) {
		levelScenes_[index]->Draw();
	}

	//コライダーの衝突判定を描画
	collisionManager_->Draw();

	if (!gameOver_) {
		//チュートリアルの描画
		drawManager_->PushBackForegroundSprite(&ballShotExplanationInfo_);
		drawManager_->PushBackForegroundSprite(&crystalExplanationInfo_);
		drawManager_->PushBackForegroundSprite(&comboExplanationInfo_);
		drawManager_->PushBackForegroundSprite(&ballLostExplanationInfo_);
	}
	if (gameOver_) {
		//玉切れの警告
		drawManager_->PushBackForegroundSprite(&gameOverInfo_);
	}
}