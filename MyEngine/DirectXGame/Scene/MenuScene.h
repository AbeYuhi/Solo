#pragma once
#include "IScene.h"
#include "DirectXGame/GameObject/Camera/MenuCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"

/// <summary>
/// Menu.h
/// メニューシーン
/// </summary>

class MenuScene : public IScene
{
public:
	MenuScene();
	~MenuScene();

	/// <summary>
	/// シーンの初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// シーンのアップデート関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// シーンの描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// シーン削除時の処理
	/// </summary>
	void Finalize() override;

private:
	MainCamera* mainCamera_;
	SpriteCamera* spriteCamera_;

};
