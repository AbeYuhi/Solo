#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(){}
PlayerBullet::~PlayerBullet(){}

void PlayerBullet::Initialize() {
	input_ = InputManager::GetInstance();

	model_ = Model::Create("sphere", "sphere.obj");
	renderItem_.Initialize();

	renderItem_.worldTransform_.data_.translate_ = MainCamera::GetInstance()->GetWorldPos();
	renderItem_.worldTransform_.data_.translate_.x += (WinApp::kWindowWidth - input_->GetMousePos().x);
}

void PlayerBullet::Update() {


}

void PlayerBullet::Draw() {
	model_->Draw(renderItem_);
}