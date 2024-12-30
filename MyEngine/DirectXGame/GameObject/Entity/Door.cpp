#include "Door.h"

/// <summary>
/// Door.cpp
/// ドアクラスの実装ファイル
/// </summary>

Door::Door() {}
Door::~Door() {}

void Door::Initialize(std::shared_ptr<MyEngine::Model> model,
	std::shared_ptr<MyEngine::RenderItem> renderItem,
	Collider* collider) {

	buttonInfo_.Initialize(model, renderItem);
	buttonCollider_ = collider;
	isOpen_ = false;
}

void Door::Update() {

	if (buttonCollider_->isContact_[BULLET]) {
		if (!isOpen_) {
			isOpen_ = true;
			moveTime_ = 0.0f;
		}
	}

	if (isOpen_) {
		if (moveTime_ <= 1.0f) {
			moveTime_ += 1.0f / 60.0f;
			leftDoorInfo_.renderItem->worldTransform_.data_.translate_.x -= 3.0f / 60.0f;
			rightDoorInfo_.renderItem->worldTransform_.data_.translate_.x += 3.0f / 60.0f;
		}
	}
}

void Door::Draw() {

	DrawManager::GetInstance()->PushBackOpaqueObject(&buttonInfo_);
	DrawManager::GetInstance()->PushBackOpaqueObject(&leftDoorInfo_);
	DrawManager::GetInstance()->PushBackOpaqueObject(&rightDoorInfo_);

}

void Door::SetRightDoor(std::shared_ptr<MyEngine::Model> model, std::shared_ptr<MyEngine::RenderItem> renderItem, Collider* collider) {
	rightDoorInfo_.Initialize(model, renderItem);
	rightDoorCollider_ = collider;
}

void Door::SetLeftDoor(std::shared_ptr<MyEngine::Model> model, std::shared_ptr<MyEngine::RenderItem> renderItem, Collider* collider) {
	leftDoorInfo_.Initialize(model, renderItem);
	leftDoorCollider_ = collider;
}