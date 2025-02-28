#include "Crystal.h"

/// <summary>
/// Crystal.cpp
/// クリスタルクラスの実装ファイル
/// </summary>

void Crystal::Initialize(std::shared_ptr<MyEngine::Model> model,
	std::shared_ptr<MyEngine::RenderItem> renderItem,
	Collider* collider) {

	info_.Initialize(model, renderItem);
	collider_ = collider;

	info_.renderItem->materialInfo_.material_->color.x = 0.5f;
	info_.renderItem->materialInfo_.material_->color.y = 0.5f;
	info_.renderItem->materialInfo_.material_->color.z = 0.5f;
	info_.renderItem->materialInfo_.material_->color.w = 0.5f;
	info_.renderItem->materialInfo_.material_->enableLightint = 1;
	info_.renderItem->materialInfo_.material_->isSpecularReflection = true;

	isBreak_ = false;

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

		//弾と衝突したら残弾の追加やコンボを増やす
		if (collider_->isContact_[BULLET] && !isBreak_) {
			*numberofSlashAttacks_ += kAmmoGain_;
			*comboDestroyCount_ += 1;
			MyEngine::AudioManager::GetInstance()->SoundPlayMp3(glassSound_, 0.7f);
			isBreak_ = true;
		}

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
	}

}

void Crystal::Draw() {
	if (!isBreak_) {
		DrawManager::GetInstance()->PushBackTranslucentObject(&info_);
	}
}