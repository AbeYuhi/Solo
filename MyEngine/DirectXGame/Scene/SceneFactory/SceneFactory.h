#pragma once
#include <memory>
#include "DirectXGame/Scene/IScene.h"
#include "DirectXGame/Scene/TitleScene.h"
#include "DirectXGame/Scene/MenuScene.h"
#include "DirectXGame/Scene/InGameScene.h"
#include "DirectXGame/Scene/GameOverScene.h"
#include "DirectXGame/Scene/GameClearScene.h"

/// <summary>
/// SceneFactory.h
/// シーンを生成するクラス
/// </summary>

class SceneFactory
{
public:

	/// <summary>
	/// インスタンスの取得関数
	/// </summary>
	/// <returns></returns>
	static SceneFactory* GetInstance();

	/// <summary>
	/// シーンの生成
	/// </summary>
	/// <returns></returns>
	std::unique_ptr<IScene> CreateScene(GameScene gameScene);

private:
	SceneFactory() = default;
	~SceneFactory() = default;
};

