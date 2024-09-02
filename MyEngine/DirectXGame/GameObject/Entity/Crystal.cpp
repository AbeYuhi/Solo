#include "Crystal.h"

void Crystal::Initialize(std::shared_ptr<Model> model,
	RenderItem* renderItem,
	Collider* collider) {

	data_.model = model;
	data_.renderItem = renderItem;
	data_.collider = collider;

	isBreak_ = false;
}

void Crystal::Update() {

	if (data_.collider->isContact_[BULLET] && !isBreak_) {
		*numberofSlashAttacks_ += 10;
		*comboDestroyCount_ += 1;
		isBreak_ = true;
	}

	if (!isBreak_) {
		if (data_.renderItem->worldTransform_.data_.translate_.z <= MainCamera::GetInstance()->GetWorldPos().z) {
			*comboDestroyCount_ = 0;
			isBreak_ = true;
		}
	}

}

void Crystal::Draw() {

	if (!isBreak_) {
		data_.model->Draw(*data_.renderItem);
	}

}