#include "Door.h"

Door::Door() {}
Door::~Door() {}

void Door::Initialize(std::shared_ptr<Model> model,
	RenderItem* renderItem,
	Collider* collider) {

	button_.model = model;
	button_.renderItem = renderItem;
	button_.collider = collider;
	isOpen_ = false;
}

void Door::Update() {

	if (button_.collider->isContact_[BULLET]) {
		if (!isOpen_) {
			isOpen_ = true;
			moveTime_ = 0.0f;
		}
	}

	if (isOpen_) {
		if (moveTime_ <= 1.0f) {
			moveTime_ += 1.0f / 60.0f;
			leftDoor_.renderItem->worldTransform_.data_.translate_.x -= 3.0f / 60.0f;
			rightDoor_.renderItem->worldTransform_.data_.translate_.x += 3.0f / 60.0f;
		}
	}

	button_.renderItem->Update();
	leftDoor_.renderItem->Update();
	rightDoor_.renderItem->Update();
}

void Door::Draw() {
	button_.model->Draw(*button_.renderItem);

	leftDoor_.model->Draw(*leftDoor_.renderItem);

	rightDoor_.model->Draw(*rightDoor_.renderItem);

}

void Door::SetRightDoor(std::shared_ptr<Model> model, RenderItem* renderItem, Collider* collider) {
	rightDoor_.model = model;
	rightDoor_.renderItem = renderItem;
	rightDoor_.collider = collider;
}

void Door::SetLeftDoor(std::shared_ptr<Model> model, RenderItem* renderItem, Collider* collider) {
	leftDoor_.model = model;
	leftDoor_.renderItem = renderItem;
	leftDoor_.collider = collider;
}