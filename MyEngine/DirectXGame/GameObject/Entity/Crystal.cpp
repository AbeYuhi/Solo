#include "Crystal.h"

/// <summary>
/// Crystal.cpp
/// クリスタルクラスの実装ファイル
/// </summary>

void Crystal::Initialize(std::shared_ptr<MyEngine::Model> model,
	MyEngine::RenderItem* renderItem,
	Collider* collider) {

	data_.model = model;
	data_.renderItem = renderItem;
	data_.collider = collider;

	data_.renderItem->materialInfo_.material_->color.x = 0.5f;
	data_.renderItem->materialInfo_.material_->color.y = 0.5f;
	data_.renderItem->materialInfo_.material_->color.z = 0.5f;
	data_.renderItem->materialInfo_.material_->color.w = 0.5f;

	isBreak_ = false;
}

void Crystal::Update() {

	if (comboDestroyCount_) {
		if (data_.collider->isContact_[BULLET] && !isBreak_) {
			*numberofSlashAttacks_ += 3;
			*comboDestroyCount_ += 1;
			isBreak_ = true;
		}

		if (!isBreak_) {
			if (data_.renderItem->worldTransform_.GetWorldPos().z <= MainCamera::GetInstance()->GetWorldPos().z) {
				*comboDestroyCount_ = 0;
				isBreak_ = true;
			}
		}
	}

	if (isBreak_) {
		data_.collider->isDelete_ = true;
	}

}

void Crystal::Draw() {

}

void Crystal::DrawTransparentObject() {

	if (!isBreak_) {
		if (Length(data_.renderItem->worldTransform_.GetWorldPos() - MainCamera::GetInstance()->GetWorldPos()) <= 100.0f) {
			data_.model->Draw(*data_.renderItem);
		}
	}

}