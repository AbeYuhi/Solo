#pragma once
#include "Base3DCamera.h"

/// <summary>
/// GameOverCamera.h
/// ゲームオーバーシーンのカメラを処理しているファイル
/// </summary>

class GameOverCamera : public Base3DCamera
{
public:
	GameOverCamera();
	~GameOverCamera();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

};

