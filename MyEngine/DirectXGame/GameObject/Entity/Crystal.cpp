#include "Crystal.h"

/// <summary>
/// Crystal.cpp
/// クリスタルクラスの実装ファイル
/// </summary>

void Crystal::Initialize(std::shared_ptr<MyEngine::Model> model,
	std::shared_ptr<MyEngine::RenderItem> renderItem,
	Collider* collider) {

	//クリスタルの初期化
	info_.Initialize(model, renderItem);
	collider_ = collider;

	info_.renderItem->materialInfo_.material_->color.x = 0.5f;
	info_.renderItem->materialInfo_.material_->color.y = 0.5f;
	info_.renderItem->materialInfo_.material_->color.z = 0.5f;
	info_.renderItem->materialInfo_.material_->color.w = 0.5f;
	info_.renderItem->materialInfo_.material_->enableLightint = 1;
	info_.renderItem->materialInfo_.material_->isSpecularReflection = true;

	particle_ = std::make_unique<GlassPieceParticle>(100);
	particle_->Initialize();
	particle_->GetEmitterPointer()->transform = info_.renderItem->worldTransform_.worldData_;

	isBreak_ = false;

	//板ポリの初期化
	planeInfo_.Initialize(MyEngine::Model::Create("plane", "plane.obj"));
	planeInfo_.renderItem->worldTransform_.data_.scale_ /= 2.0f;
	planeInfo_.renderItem->worldTransform_.data_.rotate_.y += M_PI;
	planeInfo_.renderItem->materialInfo_.isInvisible_ = true;
	planeInfo_.textureIndex = MyEngine::TextureManager::Load("crystalBreak3Shots.png");

	//ガラスが割れた時の音
	int glassSoundNum = MyEngine::RandomManager::GetInstance()->GetRandomNumber(0, 1);
	if (glassSoundNum == 0) {
		glassSound_ = MyEngine::AudioManager::GetInstance()->SoundLoadMp3("glassSound01.mp3");
	}
	if (glassSoundNum == 1) {
		glassSound_ = MyEngine::AudioManager::GetInstance()->SoundLoadMp3("glassSound02.mp3");
	}
}

void Crystal::Update() {

	if (comboDestroyCount_) {

		particle_->SetIsPopParticle(false);
		//弾と衝突したら残弾の追加やコンボを増やす
		if (collider_->isContact_[BULLET] && !isBreak_) {
			//板ポリの位置をクリスタルに合わせる
			planeInfo_.renderItem->worldTransform_.data_.translate_ = info_.renderItem->worldTransform_.worldData_.translate_;
			*numberofSlashAttacks_ += kAmmoGain_;
			*comboDestroyCount_ += 1;
			MyEngine::AudioManager::GetInstance()->SoundPlayMp3(glassSound_, kGlassSoundVolume_);
			//クリスタルが破壊されたときにパーティクルが発生
			particle_->GetEmitterPointer()->transform = info_.renderItem->worldTransform_.worldData_;
			particle_->SetIsPopParticle(true);
			isBreak_ = true;
			//スコアを加算
			player_->AddScore(kScore);
		}

		//パーティクルの更新
		particle_->Update();

		//壊されないまま通り過ぎたらコンボが途切れるように
		if (!isBreak_) {
			if (info_.renderItem->worldTransform_.GetWorldPos().z <= MainCamera::GetInstance()->GetWorldPos().z) {
				*comboDestroyCount_ = 0;
				isBreak_ = true;
			}
		}
	}

	//壊れたらコライダーマネージャーから削除するように
	if (isBreak_) {
		collider_->isDelete_ = true;
		planeInfo_.renderItem->worldTransform_.data_.translate_.y += 1.0f / 60.0f;
		planeInfo_.renderItem->materialInfo_.isInvisible_ = false;
		planeInfo_.renderItem->materialInfo_.material_->color.w -= 1.0f / 60.0f;
		if (planeInfo_.renderItem->materialInfo_.material_->color.w <= 0.0f) {
			planeInfo_.renderItem->materialInfo_.material_->color.w = 0.0f;
		}
	}

}

void Crystal::Draw() {
	if (!isBreak_) {
		//クリスタル
		DrawManager::GetInstance()->PushBackTranslucentObject(&info_);	
	}

	//クリスタルが破壊された時に出す板ポリ
	DrawManager::GetInstance()->PushBackTranslucentObject(&planeInfo_);

	//パーティクル
	DrawManager::GetInstance()->PushBackParticle(
		MyEngine::Particle([particle = particle_.get()]() { particle->Draw(); })
	);
}