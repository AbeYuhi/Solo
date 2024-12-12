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

	//ブレンドモード
	blendMode_ = kBlendModeNormal;

	std::unique_ptr<LevelScene> level0 = std::make_unique<LevelScene>();
	level0->Initialize("debug.json");
	stageSize_ = level0->GetCameraData().stageSize.z;
	levelScenes_.push_back(std::move(level0));
	std::unique_ptr<LevelScene> level1 = std::make_unique<LevelScene>();
	level1->Initialize("stage1.json", stageSize_);
	stageSize_ += level1->GetCameraData().stageSize.z;
	levelScenes_.push_back(std::move(level1));
	nowStage_ = 0;

	//インゲームカメラ
	gameCamera_ = std::make_unique<InGameCamera>();
	gameCamera_->Initialize();
	gameCamera_->transform_ = levelScenes_[0]->GetCameraData().CameraInfo;

	player_.Initialize(&gameCamera_->transform_);
	for (int i = 0; i < 2; i++) {
		for (auto& crystal : levelScenes_[i]->GetCrystals()) {
			crystal.SetComboDestroyCount(player_.GetComboDestroyCount());
			crystal.SetNumberofSlashAttacks(player_.GetNumberofSlashAttacks());
		}
	}

	ballShotExplanationTexture_ = MyEngine::TextureManager::Load("ballShot_Explanation.png");
	ballShotExplanationSprite_ = MyEngine::Sprite::Create();
	ballShotExplanationInfo_.Initialize(ballShotExplanationTexture_, { 1280, 720 }, { 0.0f, 0.0f });
	ballShotExplanationInfo_.materialInfo_.material_->color.w = 0.0f;

	crystalExplanationTexture_ = MyEngine::TextureManager::Load("crystal_Explanation.png");
	crystalExplanationSprite_ = MyEngine::Sprite::Create();
	crystalExplanationInfo_.Initialize(crystalExplanationTexture_, { 1280, 720 }, { 0.0f, 0.0f });
	crystalExplanationInfo_.materialInfo_.material_->color.w = 0.0f;

	gameOverTexture_ = MyEngine::TextureManager::Load("gameOver_Explanation.png");
	gameOverSprite_ = MyEngine::Sprite::Create();
	gameOverInfo_.Initialize(gameOverTexture_, { 1280, 720 }, { 0.0f, 0.0f });
	gameOverInfo_.materialInfo_.material_->color.w = 0.0f;
	
	backGroundTexture_ = MyEngine::TextureManager::Load("backGround.png");
	backGroundSprite_ = MyEngine::Sprite::Create();
	backGroundInfo_.Initialize(backGroundTexture_, { 1280, 720 }, { 0.0f, 0.0f });
	
	startTimer_ = 0.0f;
	ballShotRxplanationTime_ = 9999.9999f;
	crystalRxplanationTime_ = 9999.9999f;
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

		gameOverInfo_.materialInfo_.material_->color.w += 0.05f;
		if (gameOverInfo_.materialInfo_.material_->color.w > 1.0f) {
			gameOverInfo_.materialInfo_.material_->color.w = 1;
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
			cameraEasingTimer_ += 1.0f / 120.0f;
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

	postEffectManager_->SetPostEffect(kFog);

	//カメラを移動
	gameCamera_->transform_.translate_ += cameraSpeed_ * (1.0f / 60.0f);

	//チュートリアルの処理
	const float kBallShotTutorialStartTime = 4.0f;
	const float kTutorialEndTime = 2.0f;
	const float kTutorialDuration = 1.0f;
	const float kColorFadeStep = 0.05f;
	startTimer_ += 1.0f / 60.0f;
	if (startTimer_ >= kBallShotTutorialStartTime && startTimer_ <= kBallShotTutorialStartTime + kTutorialDuration) {
		ballShotExplanationInfo_.materialInfo_.material_->color.w += kColorFadeStep;
		if (ballShotExplanationInfo_.materialInfo_.material_->color.w > 1.0f) {
			ballShotExplanationInfo_.materialInfo_.material_->color.w = 1;
			isBallShotRxplanation_ = true;
			ballShotRxplanationTime_ = startTimer_;
		}
	}
	if (ballShotRxplanationTime_ + kTutorialEndTime <= startTimer_) {
		ballShotExplanationInfo_.materialInfo_.material_->color.w -= kColorFadeStep;
		if (ballShotExplanationInfo_.materialInfo_.material_->color.w < 0.0f) {
			ballShotExplanationInfo_.materialInfo_.material_->color.w = 0;
		}
	}

	const float kCrystalTutorialStartTime = 10.0f;
	if (startTimer_ >= kCrystalTutorialStartTime && startTimer_ <= kCrystalTutorialStartTime + kTutorialDuration) {
		crystalExplanationInfo_.materialInfo_.material_->color.w += kColorFadeStep;
		if (crystalExplanationInfo_.materialInfo_.material_->color.w > 1.0f) {
			crystalExplanationInfo_.materialInfo_.material_->color.w = 1;
			isCrystalRxplanation_ = true;
			crystalRxplanationTime_ = startTimer_;
		}
	}
	if (crystalRxplanationTime_ + kTutorialEndTime <= startTimer_) {
		crystalExplanationInfo_.materialInfo_.material_->color.w -= kColorFadeStep;
		if (crystalExplanationInfo_.materialInfo_.material_->color.w < 0.0f) {
			crystalExplanationInfo_.materialInfo_.material_->color.w = 0;
		}
	}

	//プレイヤーの処理
	if (!gameClear_ && !gameOver_) {
		player_.Update();
	}

	//ステージの処理
	for (int index = 0; index < levelScenes_.size(); index++) {
		levelScenes_[index]->Update();
	}

	if (player_.IsGameClear()) {
		gameClear_ = true;
	}
	if (player_.IsGameOver()) {
		gameOver_ = true;
	}
	else {
		gameOver_ = false;
	}

	collisionManager_->Update();
#ifdef _DEBUG

	ImGui::Begin("RenderItemInfo");
	ImGui::BeginTabBar("RenderItemInfo");
	ImGui::EndTabBar();
	ImGui::End();

	ImGui::Begin("BlendMode");
	const char* modes[] = { "None", "Normal", "Add", "SubTract", "MultiPly", "Screen" };
	ImGui::Combo("blendMode", &blendMode_, modes, IM_ARRAYSIZE(modes));
	MyEngine::GraphicsPipelineManager::GetInstance()->SetBlendMode(static_cast<BlendMode>(blendMode_));
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

	///背景スプライトの描画開始 
	backGroundSprite_->Draw(backGroundInfo_);

	///背景スプライト描画終了
	//深度バッファのクリア
	directXCommon_->ClearDepthStencilBuffer();

	///前面スプライトの描画開始
	///前面スプライトの描画終了

	///オブジェクトの描画開始

	player_.Draw();
	for (int index = 0; index < levelScenes_.size(); index++) {
		levelScenes_[index]->Draw();
	}
	collisionManager_->Draw();

	if (!gameOver_) {
		ballShotExplanationSprite_->Draw(ballShotExplanationInfo_);
		crystalExplanationSprite_->Draw(crystalExplanationInfo_);
	}
	if (gameOver_) {
		gameOverSprite_->Draw(gameOverInfo_);
	}

	///オブジェクトの描画終了

	///半透明オブジェクトの描画
	for (int index = 0; index < levelScenes_.size(); index++) {
		levelScenes_[index]->DrawTransparentObject();
	}
	///半透明オブジェクトの描画終了

	///パーティクルの描画

	for (int index = 0; index < levelScenes_.size(); index++) {
		levelScenes_[index]->ParticleDraw();
	}
	player_.ParticleDraw();

	///パーティクルの描画終了
}