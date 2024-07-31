#include "Door.h"

Door::Door() {}
Door::~Door() {}

void Door::Initialize(Vector3 pos, Vector3 size) {

	isOpen_ = false;

}

void Door::Update() {

	/*if (buttonCollider_.isContact_[BULLET]) {
		isOpen_ = true;
	}


	if (isOpen_) {
		leftDoorInfo_.worldTransform_.data_.translate_.x = pos_.x - size_.x - 2.0f;
		rightDoorInfo_.worldTransform_.data_.translate_.x = pos_.x + size_.x + 2.0f;
	}

	leftDoorInfo_.Update();
	rightDoorInfo_.Update();
	buttonInfo_.Update();*/
}

void Door::Draw() {/*
	model_->Draw(leftDoorInfo_);

	model_->Draw(rightDoorInfo_);

	model_->Draw(buttonInfo_);*/

}