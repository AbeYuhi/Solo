#include "MenuScene.h"

/// <summary>
/// Menu.cpp
/// メニューシーン
/// </summary>

MenuScene::MenuScene(){}
MenuScene::~MenuScene(){}

void MenuScene::Finalize() {

}

void MenuScene::Initialize() {
	sceneNo_ = MENU;

	mainCamera_ = MainCamera::GetInstance();
	spriteCamera_ = SpriteCamera::GetInstance();
}

void MenuScene::Update() {

}

void MenuScene::Draw() {

}