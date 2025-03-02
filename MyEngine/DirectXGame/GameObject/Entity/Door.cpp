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

	buttonHintLeft_.Initialize(MyEngine::Model::Create("plane", "plane.obj"));
	buttonHintLeft_.renderItem->worldTransform_.data_.translate_ = buttonInfo_.renderItem->worldTransform_.data_.translate_;
	buttonHintLeft_.renderItem->worldTransform_.data_.translate_.x -= 2.0f;
	buttonHintLeft_.renderItem->worldTransform_.data_.translate_.z -= 2.5f;
	buttonHintLeft_.renderItem->worldTransform_.data_.rotate_.y += 3.14f;
	buttonHintLeft_.renderItem->worldTransform_.data_.rotate_.z -= 3.14f / 2.0f;
	buttonHintLeft_.textureIndex = MyEngine::TextureManager::Load("buttonHint.png");

	buttonHintRight_.Initialize(MyEngine::Model::Create("plane", "plane.obj"));
	buttonHintRight_.renderItem->worldTransform_.data_.translate_ = buttonInfo_.renderItem->worldTransform_.data_.translate_;
	buttonHintRight_.renderItem->worldTransform_.data_.translate_.x += 2.0f;
	buttonHintRight_.renderItem->worldTransform_.data_.translate_.z -= 2.5f;
	buttonHintRight_.renderItem->worldTransform_.data_.rotate_.y += 3.14f;
	buttonHintRight_.renderItem->worldTransform_.data_.rotate_.z += 3.14f / 2.0f;
	buttonHintRight_.textureIndex = MyEngine::TextureManager::Load("buttonHint.png");

	//ボタンが押された時のサウンド
	buttonSound_ = MyEngine::AudioManager::GetInstance()->SoundLoadMp3("buttonSound.mp3");

	isButtonHintReturn_ = false;
	buttonHintTimer_ = 0.0f;
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
			if (moveTime_ == 0.0f) {
				MyEngine::AudioManager::GetInstance()->SoundPlayMp3(buttonSound_, 0.7f);
			}
			moveTime_ += 1.0f / 60.0f;
			leftDoorInfo_.renderItem->worldTransform_.data_.translate_.x -= 3.0f / 60.0f;
			rightDoorInfo_.renderItem->worldTransform_.data_.translate_.x += 3.0f / 60.0f;
		}
	}

	//ボタンのヒントの表示
	buttonHintTimer_ += 1.0f / 60.0f;
	if (buttonHintTimer_ > 0.25f) {	//0.25秒過ぎると反転するように
		buttonHintTimer_ -= 0.25f;
		isButtonHintReturn_ = !isButtonHintReturn_;
	}
	//矢印を動かす
	if (isButtonHintReturn_) {
		buttonHintLeft_.renderItem->worldTransform_.data_.translate_.x += 4.0f / 60.0f;
		buttonHintRight_.renderItem->worldTransform_.data_.translate_.x -= 4.0f / 60.0f;
	}
	else {
		buttonHintLeft_.renderItem->worldTransform_.data_.translate_.x -= 4.0f / 60.0f;
		buttonHintRight_.renderItem->worldTransform_.data_.translate_.x += 4.0f / 60.0f;
	}
}

void Door::Draw() {

	//ドア関連のオブジェクトの描画
	DrawManager::GetInstance()->PushBackOpaqueObject(&buttonInfo_);
	DrawManager::GetInstance()->PushBackOpaqueObject(&leftDoorInfo_);
	DrawManager::GetInstance()->PushBackOpaqueObject(&rightDoorInfo_);

	//ヒントの表示
	if (buttonInfo_.renderItem->worldTransform_.data_.translate_.z - MainCamera::GetInstance()->GetWorldPos().z <= 100.0f) {
		DrawManager::GetInstance()->PushBackOpaqueObject(&buttonHintLeft_);
		DrawManager::GetInstance()->PushBackOpaqueObject(&buttonHintRight_);
	}

}

void Door::SetRightDoor(std::shared_ptr<MyEngine::Model> model, std::shared_ptr<MyEngine::RenderItem> renderItem, Collider* collider) {
	rightDoorInfo_.Initialize(model, renderItem);
	rightDoorCollider_ = collider;
}

void Door::SetLeftDoor(std::shared_ptr<MyEngine::Model> model, std::shared_ptr<MyEngine::RenderItem> renderItem, Collider* collider) {
	leftDoorInfo_.Initialize(model, renderItem);
	leftDoorCollider_ = collider;
}