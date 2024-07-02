#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(){}
PlayerBullet::~PlayerBullet(){}

void PlayerBullet::Initialize() {
	input_ = InputManager::GetInstance();

	model_ = Model::Create("sphere", "sphere.obj");
	renderItem_.Initialize();

	
}

void PlayerBullet::Update() {


}

void PlayerBullet::Draw() {
	model_->Draw(renderItem_);
}