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

	void Initialize() override;

	void Update() override;

private:
};

