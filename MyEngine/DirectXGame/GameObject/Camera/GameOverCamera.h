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

	void Initialize() override;

	void Update() override;

private:

};

