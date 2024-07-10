#include "Door.h"

Door::Door(){}
Door::~Door(){}

void Door::Initialize(Vector3 pos, Vector3 size) {

	model_ = Model::Create("cube", "cube.obj");
	leftDoorInfo_.Initialize();
	rightDoorInfo_.Initialize();
	buttonInfo_.Initialize();

	leftDoorInfo_.worldTransform_.data_.translate_ = pos;
	leftDoorInfo_.worldTransform_.data_.translate_.x -= size.x / 2.0f;
	leftDoorInfo_.worldTransform_.data_.scale_ = size;

	rightDoorInfo_.worldTransform_.data_.translate_ = pos;
	rightDoorInfo_.worldTransform_.data_.translate_.x += size.x / 2.0f;
	rightDoorInfo_.worldTransform_.data_.scale_ = size;

	buttonInfo_.worldTransform_.data_.translate_ = pos;
	buttonInfo_.worldTransform_.data_.translate_.y += size.y / 2.0f + 1.0f;

	pos_ = pos;
	size_ = size;

	leftDoorCollider_.Initialize(&leftDoorInfo_.worldTransform_.data_.translate_, leftDoorInfo_.worldTransform_.data_.scale_, WALL, BOX, false);
	rightDoorCollider_.Initialize(&rightDoorInfo_.worldTransform_.data_.translate_, rightDoorInfo_.worldTransform_.data_.scale_, WALL, BOX, false);
	buttonCollider_.Initialize(&buttonInfo_.worldTransform_.data_.translate_, buttonInfo_.worldTransform_.data_.scale_, BUTTON, BOX, true);

	CollisionManager::GetInstance()->AddCollider(&leftDoorCollider_);
	CollisionManager::GetInstance()->AddCollider(&rightDoorCollider_);
	CollisionManager::GetInstance()->AddCollider(&buttonCollider_);

	isOpen_ = false;
}

void Door::Update() {

	if (buttonCollider_.collision_[BULLET].isContact_) {
		isOpen_ = true;
	}

	if (isOpen_) {
		leftDoorInfo_.worldTransform_.data_.translate_.x = pos_.x - size_.x - 2.0f;
		rightDoorInfo_.worldTransform_.data_.translate_.x = pos_.x + size_.x + 2.0f;
	}

	leftDoorInfo_.Update();
	rightDoorInfo_.Update();
	buttonInfo_.Update();
}

void Door::Draw() {

	model_->Draw(leftDoorInfo_);
	model_->Draw(rightDoorInfo_);
	model_->Draw(buttonInfo_);

}