#pragma once
#include <memory>
#include "DirectXGame/Base/Framework/Framework.h"
#include "DirectXGame/Object/Sprite.h"
#include "DirectXGame/Manager/SceneManager.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"

/// <summary>
/// GameManager.h
/// ゲームループを管理しているマネージャークラス
/// </summary>

class GameManager : public MyEngine::Framework
{
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static GameManager* GetInstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ゲームループ部分
	/// </summary>
	/// <returns></returns>
	int Run();

private:
	GameManager() = default;
	~GameManager() = default;

	//メインカメラ
	MainCamera* mainCamera_ = nullptr;

	//シーンマネージャー
	MyEngine::SceneManager* sceneManager_ = nullptr;

	//FPSの計測
	double deltaTime_;
	double fps_;
	int frameCount_;
	double totalTime_;
};