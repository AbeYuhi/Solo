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

	isBreak_ = false;
}

void Crystal::Update() {

	if (comboDestroyCount_) {
		if (collider_->isContact_[BULLET] && !isBreak_) {
			*numberofSlashAttacks_ += 3;
			*comboDestroyCount_ += 1;
			isBreak_ = true;
		}

		if (!isBreak_) {
			if (info_.renderItem->worldTransform_.GetWorldPos().z <= MainCamera::GetInstance()->GetWorldPos().z) {
				*comboDestroyCount_ = 0;
				isBreak_ = true;
			}
		}
	}

	if (isBreak_) {
		collider_->isDelete_ = true;
	}

}

void Crystal::Draw() {
	if (!isBreak_) {
		DrawManager::GetInstance()->PushBackTranslucentObject(&info_);
	}
}