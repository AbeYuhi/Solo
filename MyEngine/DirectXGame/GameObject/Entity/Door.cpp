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

	openDoorButtonTexture_ = MyEngine::TextureManager::Load("button", "openButtonTexture.png");
}

void Door::Update() {

	//ボールがボタンに衝突したときにドアを開く処理を有効に
	if (buttonCollider_->isContact_[BULLET]) {
		if (!isOpen_) {
			isOpen_ = true;
			buttonInfo_.textureIndex = openDoorButtonTexture_;
			moveTime_ = 0.0f;
		}
	}

	//実際に開く処理
	if (isOpen_) {
		if (moveTime_ <= 1.0f) {
			moveTime_ += 1.0f / 60.0f;
			leftDoorInfo_.renderItem->worldTransform_.data_.translate_.x -= 3.0f / 60.0f;
			rightDoorInfo_.renderItem->worldTransform_.data_.translate_.x += 3.0f / 60.0f;
		}
	}
}

void Door::Draw() {

	//ドア関連のオブジェクトの描画
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