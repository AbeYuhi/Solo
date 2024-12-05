#include "GlassPieceParticle.h"

GlassPieceParticle::GlassPieceParticle(int maxParticleCount) : ParticleManager(maxParticleCount) {}

void GlassPieceParticle::Initialize() {
	//基本機能の初期化
	ParticleManager::Initialize();

	//パーティクルの初期化
	particleModel_ = MyEngine::Model::Create("GlassPiece", "GlassPiece.obj");

	//使用するテクスチャの読み込み
	textureHandle_ = MyEngine::TextureManager::Load("whiteTexture2x2.png");

	//エミッター情報
	emitter_.transform.scale_ = { 2, 2, 2 };
	emitter_.transform.translate_ = { 0, 0, 0 };
	emitter_.count = 3;
	emitter_.frequency = 0.5;

	//ブレンドモード
	blendMode_ = kBlendModeAdd;
	preBlendMode_ = MyEngine::GraphicsPipelineManager::GetInstance()->GetBlendMode();

	drawInfo_.materialInfo_->material_->color = { 0.5f, 0.5f, 0.5f, 0.5f };

	isBillboard_ = false;
}

void GlassPieceParticle::Update() {

	//パーティクルの更新
	for (auto itParticle = particles_.begin(); itParticle != particles_.end();) {
		ParticleInfo* particle = &(*itParticle);
		//生存時間を過ぎたら
		if (particle->lifeTime <= particle->currenttime) {
			itParticle = particles_.erase(itParticle);
			continue;
		}

		//移動
		particle->velocity.y -= 4.8f * (1.0f / 60.0f);
		particle->srtData.translate_ += particle->velocity * kDeltaTime_;
		particle->currenttime += kDeltaTime_;
		particle->color.w = 0.5f - (particle->currenttime / particle->lifeTime);

		particle->srtData.rotate_ += {
		randomManager_->GetRandomNumber(-0.025f, 0.025f),
		randomManager_->GetRandomNumber(-0.025f, 0.025f),
		randomManager_->GetRandomNumber(-0.025f, 0.025f) };

		itParticle++;
	}

	//パーティクルの転送
	ParticleManager::Update();
}

void GlassPieceParticle::EmitterDraw() {
	ParticleManager::EmitterDraw();
}

void GlassPieceParticle::Draw() {

	MyEngine::GraphicsPipelineManager::GetInstance()->SetBlendMode(blendMode_);

	particleModel_->Draw(drawInfo_, textureHandle_);

	MyEngine::GraphicsPipelineManager::GetInstance()->SetBlendMode(preBlendMode_);
}

ParticleInfo GlassPieceParticle::MakeNewParticle() {
	ParticleInfo particle{};
	particle.srtData.Initialize();
	particle.srtData.translate_ = {
		randomManager_->GetRandomNumber(-emitter_.transform.scale_.x / 2.0f, emitter_.transform.scale_.x / 2.0f),
		randomManager_->GetRandomNumber(-emitter_.transform.scale_.y / 2.0f, emitter_.transform.scale_.y / 2.0f),
		randomManager_->GetRandomNumber(-emitter_.transform.scale_.z / 2.0f, emitter_.transform.scale_.z / 2.0f) };
	particle.srtData.translate_ += emitter_.transform.translate_;
	particle.srtData.scale_ = {
		randomManager_->GetRandomNumber(0.2f, 0.5f),
		randomManager_->GetRandomNumber(0.2f, 0.5f),
		glassSize_.z };
	particle.srtData.rotate_ = {
		randomManager_->GetRandomNumber(-3.14f, 3.14f),
		randomManager_->GetRandomNumber(-3.14f, 3.14f),
		randomManager_->GetRandomNumber(-3.14f, 3.14f) };
	particle.velocity = { randomManager_->GetRandomNumber(-2.0f, 2.0f), randomManager_->GetRandomNumber(-2.0f, 2.0f), randomManager_->GetRandomNumber(0.0f, 1.0f) };
	particle.color = { 0.5f, 0.5f, 0.5f, 0.75f };
	particle.lifeTime = randomManager_->GetRandomNumber(3.0f, 5.0f);
	particle.currenttime = 0.0f;
	return particle;
}