#pragma once
#include "Base3DCamera.h"

/// <summary>
/// GameClearCamera.h
/// ゲームオーバーシーンのカメラを処理しているファイル
/// </summary>

class GameClearCamera : public Base3DCamera
{
public:
	GameClearCamera();
	~GameClearCamera();

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

