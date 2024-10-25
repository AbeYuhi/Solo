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
	winApp_ = WinApp::GetInstance();
	directXCommon_ = DirectXCommon::GetInstance();
	input_ = InputManager::GetInstance();
	audioManager_ = AudioManager::GetInstance();
	randomManager_ = RandomManager::GetInstance();
	postEffectManager_ = PostEffectManager::GetInstance();
	collisionManager_ = CollisionManager::GetInstance();
	mainCamera_ = MainCamera::GetInstance();
	spriteCamera_ = SpriteCamera::GetInstance();

	//スプライトカメラの初期化
	spriteCamera_->Initialize();

	//平行光源
	lightObj_ = LightObjectManager::GetInstance();
	lightObj_->InitData();

	//影
	shadow_ = std::make_unique<Shadow>();
	shadow_->Initialize();

	//コリジョンマネージャー
	collisionManager_->Initialize();

	//ブレンドモード
	blendMode_ = kBlendModeNormal;

	levelScene_.Initialize("test.json", 1);
	stage0Scene_.Initialize("glassTest.json");

	//インゲームカメラ
	gameCamera_ = std::make_unique<InGameCamera>();
	gameCamera_->Initialize();
	//gameCamera_->transform_ = levelScene_.GetCameraData().CameraInfo;
	gameCamera_->transform_ = stage0Scene_.GetCameraData().CameraInfo;

	player_.Initialize(&gameCamera_->transform_);
	for (auto& crystal : levelScene_.GetCrystals()) {
		crystal.SetComboDestroyCount(player_.GetComboDestroyCount());
		crystal.SetNumberofSlashAttacks(player_.GetNumberofSlashAttacks());
	}

	ballShotExplanationTexture_ = TextureManager::Load("ballShot_Explanation.png");
	ballShotExplanationSprite_ = Sprite::Create();
	ballShotExplanationInfo_.Initialize(ballShotExplanationTexture_, { 1280, 720 }, { 0.0f, 0.0f });
	ballShotExplanationInfo_.materialInfo_.material_->color.w = 0.0f;

	crystalExplanationTexture_ = TextureManager::Load("crystal_Explanation.png");
	crystalExplanationSprite_ = Sprite::Create();
	crystalExplanationInfo_.Initialize(crystalExplanationTexture_, { 1280, 720 }, { 0.0f, 0.0f });
	crystalExplanationInfo_.materialInfo_.material_->color.w = 0.0f;
	
	startTimer_ = 0.0f;
	ballShotRxplanationTime_ = 9999.9999f;
	crystalRxplanationTime_ = 9999.9999f;
	cameraSpeed_ = { 0.0f, 0.0f, 0.0f };
	cameraEasingTimer_ = 0.0f;
	easingTimer_ = 0.0f;
}

void InGameScene::Update() {
	//スプライトカメラの更新
	spriteCamera_->Update();
	mainCamera_->Update(gameCamera_->GetWorldTransrom(), gameCamera_->GetWorldMatrix(), gameCamera_->GetProjectionMatrix());
	//ライトの更新
	lightObj_->Update();
	//影の更新
	shadow_->Update(lightObj_->GetDirectionalLightData(0).direction);
	player_.Update();

	if (startTimer_ >= 1.0f) {
		cameraEasingTimer_ += 1.0f / 120.0f;
		if (cameraEasingTimer_ > 1.0f) {
			cameraEasingTimer_ = 1.0f;
		}

		easingTimer_ = 1 - std::cos((cameraEasingTimer_ * M_PI) / 2);
		cameraSpeed_.z = easingTimer_ * 5.0f;
	}

	gameCamera_->transform_.translate_ += cameraSpeed_ * (1.0f / 60.0f);

	startTimer_ += 1.0f / 60.0f;
	if (startTimer_ >= 2.0f && startTimer_ <= 3.0f) {
		ballShotExplanationInfo_.materialInfo_.material_->color.w += 0.05f;
		if (ballShotExplanationInfo_.materialInfo_.material_->color.w > 1.0f) {
			ballShotExplanationInfo_.materialInfo_.material_->color.w = 1;
			isBallShotRxplanation_ = true;
			ballShotRxplanationTime_ = startTimer_;
		}
	}
	if (ballShotRxplanationTime_ + 2.0f <= startTimer_) {
		ballShotExplanationInfo_.materialInfo_.material_->color.w -= 0.05f;
		if (ballShotExplanationInfo_.materialInfo_.material_->color.w < 0.0f) {
			ballShotExplanationInfo_.materialInfo_.material_->color.w = 0;
		}
	}

	if (startTimer_ >= 8.0f && startTimer_ <= 9.0f) {
		crystalExplanationInfo_.materialInfo_.material_->color.w += 0.05f;
		if (crystalExplanationInfo_.materialInfo_.material_->color.w > 1.0f) {
			crystalExplanationInfo_.materialInfo_.material_->color.w = 1;
			isCrystalRxplanation_ = true;
			crystalRxplanationTime_ = startTimer_;
		}
	}
	if (crystalRxplanationTime_ + 2.0f <= startTimer_) {
		crystalExplanationInfo_.materialInfo_.material_->color.w -= 0.05f;
		if (crystalExplanationInfo_.materialInfo_.material_->color.w < 0.0f) {
			crystalExplanationInfo_.materialInfo_.material_->color.w = 0;
		}
	}

	gameCamera_->Update();

	levelScene_.Update();
	stage0Scene_.Update();
	
	if (player_.IsGameClear()) {
		sceneNo_ = GAMECLEAR;
	}
	if (player_.IsGameOver()) {
		sceneNo_ = GAMEOVER;
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
	GraphicsPipelineManager::GetInstance()->SetBlendMode(static_cast<BlendMode>(blendMode_));
	ImGui::End();

	ImGui::Begin("PostEffect");
	const char* postEffects[] = { "None", "Copy", "HSVFilter", "GrayScale", "SepiaScale", "Vignette", "Smoothing"};
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
	ImGui::EndTabBar();

	ImGui::End();

#endif // _DEBUG
}

void InGameScene::Draw() {

	///背景スプライトの描画開始 

	

	///背景スプライト描画終了
	//深度バッファのクリア
	directXCommon_->ClearDepthStencilBuffer();

	///前面スプライトの描画開始
	///前面スプライトの描画終了

	///オブジェクトの描画開始

	player_.Draw();
	levelScene_.Draw();
	stage0Scene_.Draw();
	collisionManager_->Draw();

	ballShotExplanationSprite_->Draw(ballShotExplanationInfo_);
	crystalExplanationSprite_->Draw(crystalExplanationInfo_);

	///オブジェクトの描画終了

	///パーティクルの描画

	//testParticle1_->Draw();

	///パーティクルの描画終了
}